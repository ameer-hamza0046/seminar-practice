#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <linux/if_link.h>
#include <poll.h>
#include <assert.h>

#include <bpf/bpf.h>
#include <xdp/xsk.h>
#include <xdp/libxdp.h>

#define NUM_FRAMES 4096
#define FRAME_SIZE XSK_UMEM__DEFAULT_FRAME_SIZE
#define RX_BATCH_SIZE 64
#define INVALID_UMEM_FRAME UINT64_MAX
#define IF_NAMESIZE 12

const char* if_name = "lo";

struct xsk_umem_info {
	struct xsk_ring_prod fq;
	struct xsk_ring_cons cq;
	struct xsk_umem *umem;
	void *buffer;
};

struct stats_record {
	uint64_t timestamp;
	uint64_t rx_packets;
	uint64_t rx_bytes;
	uint64_t tx_packets;
	uint64_t tx_bytes;
};

struct xsk_socket_info {
	struct xsk_ring_cons rx;
	struct xsk_ring_prod tx;
	struct xsk_umem_info *umem;
	struct xsk_socket *xsk;

	uint64_t umem_frame_addr[NUM_FRAMES];
	uint32_t umem_frame_free;

	struct stats_record stats;
};

static struct xsk_umem_info *configure_xsk_umem(void* buffer, uint64_t size)
{
    struct xsk_umem_info *umem;
    int ret;

    umem = malloc(sizeof(struct xsk_umem_info));
    if(!umem) {
        return NULL;
    }
    
    ret = xsk_umem__create(&umem->umem, buffer, size, &umem->fq, &umem->cq, NULL);
    if(ret) {
        printf("ERROR: xsk_umem__create\n");
        return NULL;
    }
    umem->buffer = buffer;
    return umem;
}

static uint64_t xsk_alloc_umem_frame(struct xsk_socket_info *xsk)
{
	uint64_t frame;
	if (xsk->umem_frame_free == 0)
		return INVALID_UMEM_FRAME;

	frame = xsk->umem_frame_addr[--xsk->umem_frame_free];
	xsk->umem_frame_addr[xsk->umem_frame_free] = INVALID_UMEM_FRAME;
	return frame;
}

static struct xsk_socket_info *xsk_configure_socket(struct xsk_umem_info *umem)
{
    struct xsk_socket_config xsk_cfg;
    struct xsk_socket_info *xsk_info;
    uint32_t idx;
    int i;
    int ret;

    xsk_info = malloc(sizeof(struct xsk_socket_info));
    if(!xsk_info) {
        return NULL;
    }
    xsk_info->umem = umem;
    xsk_cfg.rx_size = XSK_RING_CONS__DEFAULT_NUM_DESCS;
    xsk_cfg.tx_size = XSK_RING_PROD__DEFAULT_NUM_DESCS;
    xsk_cfg.xdp_flags = XDP_FLAGS_SKB_MODE;
    xsk_cfg.bind_flags = 0;
    xsk_cfg.libxdp_flags = 0;
    ret = xsk_socket__create(&xsk_info->xsk, if_name, 0, umem->umem, &xsk_info->rx, &xsk_info->tx, &xsk_cfg);
    if(ret) {
        perror("xsk_socket__create() failed");
        return NULL;
    }

    for(i = 0; i < NUM_FRAMES; i++)
    {
        xsk_info->umem_frame_addr[i] = i * FRAME_SIZE;
    }
    xsk_info->umem_frame_free = NUM_FRAMES;

    ret = xsk_ring_prod__reserve(&xsk_info->umem->fq, XSK_RING_PROD__DEFAULT_NUM_DESCS, &idx);
    if(ret != XSK_RING_PROD__DEFAULT_NUM_DESCS)
    {
        perror("xsk_ring_prod__reserve failed");
        return NULL;
    }

    for(i =0; i<XSK_RING_PROD__DEFAULT_NUM_DESCS; i++)
    {
        *xsk_ring_prod__fill_addr(&xsk_info->umem->fq, idx++) = xsk_alloc_umem_frame(xsk_info);

        xsk_ring_prod__submit(&xsk_info->umem->fq, XSK_RING_PROD__DEFAULT_NUM_DESCS);
    }
    return xsk_info;
}

static void handle_receive_packets(struct xsk_socket_info *xsk)
{
    unsigned int rcvd, stock_frames, i;
    uint32_t idx_rx = 0, idx_fq = 0;
    int ret;

    /////////////////////
    rcvd = xsk_ring_cons__peek(&xsk->rx, RX_BATCH_SIZE, &idx_rx);
    if(!rcvd)
        return;
    
    //////////////// get the packets
    stock_frames = xsk_prod_nb_free(&xsk->umem->fq, xsk->umem_frame_free); // ???

    if(stock_frames > 0)
    {
        rcvd = ret = xsk_ring_prod__reserve(&xsk->umem->fq, 
        stock_frames, &idx_fq);

        while(ret != stock_frames)
        {
            ret = xsk_ring_prod__reserve(&xsk->umem->fq, rcvd, &idx_fq);
        }

        for (i = 0; i < stock_frames; i++)
			*xsk_ring_prod__fill_addr(&xsk->umem->fq, idx_fq++) = xsk_alloc_umem_frame(xsk);
            
            xsk_ring_prod__submit(&xsk->umem->fq, stock_frames);
    }


    ///////////////// process the packets
    for (i = 0; i < rcvd; i++) {
		uint64_t addr = xsk_ring_cons__rx_desc(&xsk->rx, idx_rx)->addr;
		uint32_t len = xsk_ring_cons__rx_desc(&xsk->rx, idx_rx++)->len;
        
		// if (!process_packet(xsk, addr, len))
        // free the umem_frames
        assert(xsk->umem_frame_free < NUM_FRAMES);
        xsk->umem_frame_addr[xsk->umem_frame_free++] = addr;

		xsk->stats.rx_bytes += len;
	}

	xsk_ring_cons__release(&xsk->rx, rcvd);
	xsk->stats.rx_packets += rcvd;

    printf("Number of packets received: %lu\n", xsk->stats.rx_packets);

	// /* Do we need to wake up the kernel for transmission */
	// complete_tx(xsk);
}

static void rx_and_process(struct xsk_socket_info *xsk_socket)
{
    struct pollfd fds[2];
    int ret, nfds = 1;

    memset(fds, 0, sizeof(fds));
    fds[0].fd = xsk_socket__fd(xsk_socket->xsk);
    fds[0].events = POLLIN;

    while(1)
    {
        ret = poll(fds, nfds, -1);
        // printf("hello ret: %d\n", ret);
        if(ret <= 0 || ret > 1)
            continue;
        handle_receive_packets(xsk_socket);
    }
}

void handle_sigint(int signal) {
    printf("Exiting...\n");
    exit(0);
}

int main() {
    struct xsk_umem_info *umem;
    struct xsk_socket_info *xsk_socket;

    void* packet_buffer;
    uint64_t packet_buffer_size;

    signal(SIGINT, handle_sigint);

    packet_buffer_size = NUM_FRAMES * FRAME_SIZE;
    if(posix_memalign(&packet_buffer, getpagesize(), packet_buffer_size)) {
        printf("ERROR: Cannot allocate buffer memory\n");
        exit(EXIT_FAILURE);
    }

    umem = configure_xsk_umem(packet_buffer, packet_buffer_size);
    if(umem == NULL) {
        perror("configure_xsk_umem failed");
        exit(EXIT_FAILURE);
    }

    xsk_socket = xsk_configure_socket(umem);
    if(xsk_socket == NULL) {
        printf("ERROR: Cannot setup AF_XDP socket\n");
        exit(EXIT_FAILURE);
    }

    rx_and_process(xsk_socket);
    return 0;
}