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
const char *ifname = "wlo1";

void handle_sigint(int signal) {
  printf("SIGINT... Exiting...\n");
  exit(0);
}

int main() {
  // handle SIGINT
  signal(SIGINT, handle_sigint);

  // Initialize umem_area / buffer
  void *umem_area;
  uint64_t size;

  size = NUM_FRAMES * FRAME_SIZE;
  if (posix_memalign(&umem_area, getpagesize(), size)) {
    perror("posix_memalign");
    exit(EXIT_FAILURE);
  }

  // Configure UMEM
  struct xsk_umem *umem;
  struct xsk_ring_prod fill;
  struct xsk_ring_cons comp;
  if (xsk_umem__create(&umem, umem_area, size, &fill, &comp, NULL)) {
    perror("xsk_umem__create");
    exit(EXIT_FAILURE);
  }

  // Configure Socket
  struct xsk_socket *xsk;
  struct xsk_ring_cons rx;
  struct xsk_ring_prod tx;
  struct xsk_socket_config config;

  config.rx_size = XSK_RING_CONS__DEFAULT_NUM_DESCS;
  config.tx_size = XSK_RING_PROD__DEFAULT_NUM_DESCS;
  config.xdp_flags = XDP_FLAGS_SKB_MODE;
  config.bind_flags = 0;
  config.libxdp_flags = 0;

  if (xsk_socket__create(&xsk, ifname, 0, umem, &rx, &tx, &config)) {
    perror("xsk_socket__create");
    exit(EXIT_FAILURE);
  }

  
  printf("done\n");
  return 0;
}