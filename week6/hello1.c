#include <assert.h>
#include <errno.h>
#include <linux/if_link.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <bpf/bpf.h>
#include <xdp/libxdp.h>
#include <xdp/xsk.h>

#define NUM_FRAMES 4096
#define FRAME_SIZE XSK_UMEM__DEFAULT_FRAME_SIZE
#define INVALID_UMEM_FRAME UINT64_MAX
#define RX_BATCH_SIZE 64
const char *ifname = "wlo1";

void handle_sigint(int signal) {
  printf("SIGINT... Exiting...\n");
  exit(0);
}

// UMEM
void *umem_area;
uint64_t size;
struct xsk_umem *umem;
struct xsk_ring_prod fill;
struct xsk_ring_cons comp;
uint64_t umem_frame_addr[NUM_FRAMES];
uint32_t umem_frame_free;

// XDP SOCKET
struct xsk_socket *xsk;
struct xsk_ring_cons rx;
struct xsk_ring_prod tx;
struct xsk_socket_config config;

// VARS
uint32_t ret, stock_frames, idx;

// FUNCTIONS
uint64_t get_frame_addr() {
  uint64_t frame;
  if (umem_frame_free == 0) {
    frame = INVALID_UMEM_FRAME;
  } else {
    umem_frame_free--;
    frame = umem_frame_addr[umem_frame_free];
    umem_frame_addr[umem_frame_free] = INVALID_UMEM_FRAME;
  }
  return frame;
}

int main() {
  // handle SIGINT
  signal(SIGINT, handle_sigint);

  // Initialize umem_area / buffer
  size = NUM_FRAMES * FRAME_SIZE;
  if (posix_memalign(&umem_area, getpagesize(), size)) {
    perror("posix_memalign");
    exit(EXIT_FAILURE);
  }

  // Configure UMEM
  if (xsk_umem__create(&umem, umem_area, size, &fill, &comp, NULL)) {
    perror("xsk_umem__create");
    exit(EXIT_FAILURE);
  }

  // Configure Socket
  config.rx_size = XSK_RING_CONS__DEFAULT_NUM_DESCS;
  config.tx_size = XSK_RING_PROD__DEFAULT_NUM_DESCS;
  config.xdp_flags = XDP_FLAGS_SKB_MODE;
  config.bind_flags = 0;
  config.libxdp_flags = 0;

  if (xsk_socket__create(&xsk, ifname, 0, umem, &rx, &tx, &config)) {
    perror("xsk_socket__create");
    exit(EXIT_FAILURE);
  }

  // Initialize umem_frames
  for (int i = 0; i < NUM_FRAMES; i++) {
    umem_frame_addr[i] = i * FRAME_SIZE;
  }
  umem_frame_free = NUM_FRAMES;

  idx = 0;
  stock_frames = xsk_prod_nb_free(&fill, umem_frame_free);
  ret = xsk_ring_prod__reserve(&fill, stock_frames, &idx);
  printf("stock: %d, reserved: %u\n", stock_frames, ret);
  if (ret != stock_frames) {
    perror("xsk_ring_prod__reserve");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < stock_frames; i++) {
    *xsk_ring_prod__fill_addr(&fill, idx++) = get_frame_addr();
  }
  xsk_ring_prod__submit(&fill, stock_frames);

  // struct pollfd fds[2];
  // int nfds = 1;
  // memset(fds, 0, sizeof(fds));
  // fds[0].fd = xsk_socket__fd(xsk);
  // fds[0].events = POLLIN;

  // while (1) {
  //   ret = poll(fds, nfds, -1);
  //   if (ret != 1)
  //     continue;

  //   uint32_t rcvd, idx_rx = 0, stock_frames, idx_fill = 0;
  //   rcvd = xsk_ring_cons__peek(&rx, RX_BATCH_SIZE, &idx_rx);
  //   if (!rcvd)
  //     continue;

  //   stock_frames = xsk_prod_nb_free(&fill, umem_frame_free);
  //   if (stock_frames > 0) {
  //     while (ret != stock_frames) {
  //       ret = xsk_ring_prod__reserve(&fill, rcvd, &idx_fill);
  //     }
  //   }
  // }

  printf("done\n");
  return 0;
}