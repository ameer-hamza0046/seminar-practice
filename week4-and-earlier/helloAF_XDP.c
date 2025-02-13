
int main() {
  return 0;
}





// #include <errno.h>
// #include <getopt.h>
// #include <libgen.h>
// #include <linux/bpf.h>
// // #include <linux/err.h>
// #include <arpa/inet.h>
// #include <linux/if_ether.h>
// #include <linux/if_link.h>
// #include <linux/if_xdp.h>
// #include <linux/ip.h>
// #include <linux/limits.h>
// #include <linux/udp.h>
// #include <locale.h>
// #include <net/ethernet.h>
// #include <net/if.h>
// #include <netinet/ether.h>
// #include <poll.h>
// #include <pthread.h>
// #include <sched.h>
// #include <signal.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/capability.h>
// #include <sys/mman.h>
// #include <sys/resource.h>
// #include <sys/socket.h>
// #include <sys/types.h>
// #include <sys/un.h>
// #include <time.h>
// #include <unistd.h>

// // #include <xdp/xsk.h>
// // #include <xdp/libxdp.h>

// #include <bpf/bpf.h>
// #include <bpf/libbpf.h>

// #ifndef XDPSOCK_H_
// #define XDPSOCK_H_

// #define MAX_SOCKS 4

// #define SOCKET_NAME "sock_cal_bpf_fd"
// #define MAX_NUM_OF_CLIENTS 10

// #define CLOSE_CONN 1

// typedef __u64 u64;
// typedef __u32 u32;

// #endif /* XDPSOCK_H */

// #ifndef SOL_XDP
// #define SOL_XDP 283
// #endif

// #ifndef AF_XDP
// #define AF_XDP 44
// #endif

// #ifndef PF_XDP
// #define PF_XDP AF_XDP
// #endif

// #ifndef SO_PREFER_BUSY_POLL
// #define SO_PREFER_BUSY_POLL 69
// #endif

// #ifndef SO_BUSY_POLL_BUDGET
// #define SO_BUSY_POLL_BUDGET 70
// #endif

// #define NUM_FRAMES (4 * 1024)
// #define MIN_PKT_SIZE 64
// #define MAX_PKT_SIZE 9728 /* Max frame size supported by many NICs */
// #define IS_EOP_DESC(options) (!((options) & XDP_PKT_CONTD))

// #define DEBUG_HEXDUMP 0

// #define VLAN_PRIO_MASK 0xe000 /* Priority Code Point */
// #define VLAN_PRIO_SHIFT 13
// #define VLAN_VID_MASK 0x0fff /* VLAN Identifier */
// #define VLAN_VID__DEFAULT 1
// #define VLAN_PRI__DEFAULT 0

// #define NSEC_PER_SEC 1000000000UL
// #define NSEC_PER_USEC 1000

// #define SCHED_PRI__DEFAULT 0
// #define STRERR_BUFSIZE 1024

// typedef __u64 u64;
// typedef __u32 u32;
// typedef __u16 u16;
// typedef __u8 u8;

// static unsigned long prev_time;
// static long tx_cycle_diff_min;
// static long tx_cycle_diff_max;
// static double tx_cycle_diff_ave;
// static long tx_cycle_cnt;

// enum benchmark_type {
//   BENCH_RXDROP = 0,
//   BENCH_TXONLY = 1,
//   BENCH_L2FWD = 2,
// };

// static enum benchmark_type opt_bench = BENCH_RXDROP;
// static enum xdp_attach_mode opt_attach_mode = XDP_MODE_NATIVE;
// static const char *opt_if = "";
// static int opt_ifindex;
// static int opt_queue;
// static unsigned long opt_duration;
// static unsigned long start_time;
// static bool benchmark_done;
// static u32 opt_batch_size = 64;
// static int opt_pkt_count;
// static u16 opt_pkt_size = MIN_PKT_SIZE;
// static u32 opt_pkt_fill_pattern = 0x12345678;
// static bool opt_vlan_tag;
// static u16 opt_pkt_vlan_id = VLAN_VID__DEFAULT;
// static u16 opt_pkt_vlan_pri = VLAN_PRI__DEFAULT;
// static struct ether_addr opt_txdmac = {{0x3c, 0xfd, 0xfe, 0x9e, 0x7f, 0x71}};
// static struct ether_addr opt_txsmac = {{0xec, 0xb1, 0xd7, 0x98, 0x3a, 0xc0}};
// static bool opt_extra_stats;
// static bool opt_quiet;
// static bool opt_app_stats;
// static const char *opt_irq_str = "";
// static u32 irq_no;
// static int irqs_at_init = -1;
// static u32 sequence;
// static int opt_poll;
// static int opt_interval = 1;
// static int opt_retries = 3;
// static u32 opt_xdp_bind_flags = XDP_USE_NEED_WAKEUP;
// static u32 opt_umem_flags;
// static int opt_unaligned_chunks;
// static int opt_mmap_flags;
// static int opt_xsk_frame_size = XSK_UMEM__DEFAULT_FRAME_SIZE;
// static int frames_per_pkt;
// static int opt_timeout = 1000;
// static bool opt_need_wakeup = true;
// static u32 opt_num_xsks = 1;
// static bool opt_busy_poll;
// static bool opt_reduced_cap;
// static clockid_t opt_clock = CLOCK_MONOTONIC;
// static unsigned long opt_tx_cycle_ns;
// static int opt_schpolicy = SCHED_OTHER;
// static int opt_schprio = SCHED_PRI__DEFAULT;
// static bool opt_tstamp;
// static struct xdp_program *xdp_prog;
// static bool opt_frags;
// static bool load_xdp_prog;

// struct vlan_ethhdr {
//   unsigned char h_dest[6];
//   unsigned char h_source[6];
//   __be16 h_vlan_proto;
//   __be16 h_vlan_TCI;
//   __be16 h_vlan_encapsulated_proto;
// };

// #define PKTGEN_MAGIC 0xbe9be955
// struct pktgen_hdr {
//   __be32 pgh_magic;
//   __be32 seq_num;
//   __be32 tv_sec;
//   __be32 tv_usec;
// };

// struct xsk_ring_stats {
//   unsigned long rx_frags;
//   unsigned long rx_npkts;
//   unsigned long tx_frags;
//   unsigned long tx_npkts;
//   unsigned long rx_dropped_npkts;
//   unsigned long rx_invalid_npkts;
//   unsigned long tx_invalid_npkts;
//   unsigned long rx_full_npkts;
//   unsigned long rx_fill_empty_npkts;
//   unsigned long tx_empty_npkts;
//   unsigned long prev_rx_frags;
//   unsigned long prev_rx_npkts;
//   unsigned long prev_tx_frags;
//   unsigned long prev_tx_npkts;
//   unsigned long prev_rx_dropped_npkts;
//   unsigned long prev_rx_invalid_npkts;
//   unsigned long prev_tx_invalid_npkts;
//   unsigned long prev_rx_full_npkts;
//   unsigned long prev_rx_fill_empty_npkts;
//   unsigned long prev_tx_empty_npkts;
// };

// struct xsk_driver_stats {
//   unsigned long intrs;
//   unsigned long prev_intrs;
// };

// struct xsk_app_stats {
//   unsigned long rx_empty_polls;
//   unsigned long fill_fail_polls;
//   unsigned long copy_tx_sendtos;
//   unsigned long tx_wakeup_sendtos;
//   unsigned long opt_polls;
//   unsigned long prev_rx_empty_polls;
//   unsigned long prev_fill_fail_polls;
//   unsigned long prev_copy_tx_sendtos;
//   unsigned long prev_tx_wakeup_sendtos;
//   unsigned long prev_opt_polls;
// };

// struct xsk_umem_info {
//   struct xsk_ring_prod fq;
//   struct xsk_ring_cons cq;
//   struct xsk_umem *umem;
//   void *buffer;
// };

// struct xsk_socket_info {
//   struct xsk_ring_cons rx;
//   struct xsk_ring_prod tx;
//   struct xsk_umem_info *umem;
//   struct xsk_socket *xsk;
//   struct xsk_ring_stats ring_stats;
//   struct xsk_app_stats app_stats;
//   struct xsk_driver_stats drv_stats;
//   u32 outstanding_tx;
// };

// static const struct sched_map {
//   const char *name;
//   int policy;
// } schmap[] = {{"OTHER", SCHED_OTHER}, {"FIFO", SCHED_FIFO}, {NULL}};

// static int num_socks;
// struct xsk_socket_info *xsks[MAX_SOCKS];
// int sock;

// int main() { return 0; }

// /* SPDX-License-Identifier: GPL-2.0 */

// #include <assert.h>
// #include <errno.h>
// #include <getopt.h>
// #include <locale.h>
// #include <poll.h>
// #include <pthread.h>
// #include <signal.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>
// #include <unistd.h>

// #include <sys/resource.h>

// #include "xdp-tools/headers/xdp/libxdp.h"
// #include "xdp-tools/headers/xdp/xsk.h"
// #include "xdp-tools/xdp-filter/xdp-filter.c"
// #include <bpf/bpf.h>

// #include <arpa/inet.h>
// #include <linux/icmpv6.h>
// #include <linux/if_ether.h>
// #include <linux/if_link.h>
// #include <linux/ipv6.h>
// #include <net/if.h>

// #include "xdp-tutorial/common/common_libbpf.h"
// #include "xdp-tutorial/common/common_params.h"
// #include "xdp-tutorial/common/common_user_bpf_xdp.h"

// #define NUM_FRAMES 4096
// #define FRAME_SIZE XSK_UMEM__DEFAULT_FRAME_SIZE
// #define RX_BATCH_SIZE 64
// #define INVALID_UMEM_FRAME UINT64_MAX

// struct wxsk_umem_info {
//   struct xsk_ring_prod fq;
//   struct xsk_ring_cons cq;
//   struct xsk_umem *umem;
//   void *buffer;
// };

// struct stats_record {
//   uint64_t timestamp;
//   uint64_t rx_packets;
//   uint64_t rx_bytes;
//   uint64_t tx_packets;
//   uint64_t tx_bytes;
// };

// struct xsk_socket_info {
//   struct xsk_ring_cons rx;
//   struct xsk_ring_prod tx;
//   struct xsk_umem_info *umem;
//   struct xsk_socket *xsk;

//   uint64_t umem_frame_addr[NUM_FRAMES];
//   uint32_t umem_frame_free;

//   uint32_t outstanding_tx;

//   struct stats_record stats;
//   struct stats_record prev_stats;
// };

// struct config cfg = {
// 	.ifindex   = -1,
// };

// static bool global_exit;

// static void exit_application(int signal) {
//   int err;

//   cfg.unload_all = true;
//   err = do_unload(&cfg);
//   if (err) {
//     fprintf(stderr, "Couldn't detach XDP program on iface '%s' : (%d)\n",
//             cfg.ifname, err);
//   }

//   signal = signal;
//   global_exit = true;
// }

// int main() {
//   int ret;
//   void *packet_buffer;
//   uint64_t packet_buffer_size;
//   DECLARE_LIBBPF_OPTS(bpf_object_open_opts, opts);
//   DECLARE_LIBXDP_OPTS(xdp_program_opts, xdp_opts, 0);
//   struct rlimit rlim = {RLIM_INFINITY, RLIM_INFINITY};
//   struct xsk_umem_info *umem;
//   struct xsk_socket_info *xsk_socket;
//   int err;
//   char errmsg[1024];

//   signal(SIGINT, exit_application);

//   // signal(SIGINT, handle_sigint);
//   // sock = socket(AF_XDP, SOCK_RAW, 0);
//   // if (sock < 0) {
//   //   perror("socket");
//   //   return 1;
//   // }
//   // struct sockaddr_xdp sxdp = {0};
//   // sxdp.sxdp_family = AF_XDP;
//   // sxdp.sxdp_ifindex = if_nametoindex(IFACE);

//   // if (bind(sock, (struct sockaddr *)&sxdp, sizeof(sxdp)) < 0) {
//   //   perror("bind");
//   //   close(sock);
//   //   return 1;
//   // }

//   // printf("AF_XDP socket bound to %s, waiting for packets...\n", IFACE);

//   // char buf[2048];
//   // while (1) {
//   //   int len = recv(sock, buf, sizeof(buf), 0);
//   //   if (len > 0) {
//   //     printf("Received %d bytes\n", len);
//   //   }
//   // }

//   // close(sock);
//   // return 0;
// }
// // #include <bpf/xsk.h>
// // #include <errno.h>
// // #include <linux/if_link.h>
// // #include <linux/if_xdp.h>
// // #include <net/if.h>
// // #include <signal.h>
// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <string.h>
// // #include <sys/socket.h>
// // #include <unistd.h>

// // #define IFACE "wlo1"

// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <string.h>
// // #include <unistd.h>
// // #include <signal.h>
// // #include <bpf/bpf.h>
// // #include <bpf/libbpf.h>
// // #include <xdp/libxdp.h>
// // #include <linux/if_link.h>
// // #include <linux/if_xdp.h>
// // #include <linux/if_ether.h>
// // #include <arpa/inet.h>

// // #define NUM_FRAMES 2048
// // #define FRAME_SIZE 2048
// // #define BATCH_SIZE 64

// // static volatile bool running = true;

// // void sigint_handler(int sig) {
// //     running = false;
// // }

// // int main(int argc, char **argv) {
// //     struct xsk_socket_config cfg;
// //     struct xsk_umem_config umem_cfg;
// //     struct xsk_umem *umem;
// //     struct xsk_socket *xsk;
// //     void *umem_area;
// //     int ret;

// //     if (argc != 2) {
// //         printf("Usage: %s <interface>\n", argv[0]);
// //         return 1;
// //     }

// //     const char *ifname = argv[1];

// //     // Allocate memory for UMEM
// //     umem_area = malloc(NUM_FRAMES * FRAME_SIZE);
// //     if (!umem_area) {
// //         perror("malloc");
// //         return 1;
// //     }

// //     // Configure UMEM
// //     memset(&umem_cfg, 0, sizeof(umem_cfg));
// //     umem_cfg.fill_size = NUM_FRAMES;
// //     umem_cfg.comp_size = NUM_FRAMES;
// //     umem_cfg.frame_size = FRAME_SIZE;
// //     umem_cfg.frame_headroom = 0;

// //     ret = xsk_umem__create(&umem, umem_area, NUM_FRAMES * FRAME_SIZE,
// //                            &umem_cfg.fill_ring, &umem_cfg.comp_ring,
// //                            &umem_cfg);
// //     if (ret) {
// //         fprintf(stderr, "Failed to create UMEM: %d\n", ret);
// //         free(umem_area);
// //         return 1;
// //     }

// //     // Configure XSK
// //     memset(&cfg, 0, sizeof(cfg));
// //     cfg.rx_size = NUM_FRAMES;
// //     cfg.tx_size = NUM_FRAMES;
// //     cfg.libbpf_flags = 0;
// //     cfg.xdp_flags = XDP_FLAGS_UPDATE_IF_NOEXIST;
// //     cfg.bind_flags = XDP_ZEROCOPY;

// //     ret = xsk_socket__create(&xsk, ifname, 0, umem, &umem_cfg.rx_ring,
// //     &umem_cfg.tx_ring, &cfg); if (ret) {
// //         fprintf(stderr, "Failed to create XSK socket: %d\n", ret);
// //         xsk_umem__delete(umem);
// //         free(umem_area);
// //         return 1;
// //     }

// //     // Set up signal handler for graceful exit
// //     signal(SIGINT, sigint_handler);

// //     unsigned long packet_count = 0;
// //     struct xdp_desc descs[BATCH_SIZE];

// //     printf("Counting packets on interface %s...\n", ifname);

// //     while (running) {
// //         // Receive packets
// //         unsigned int rcvd = xsk_ring_cons__peek(&umem_cfg.rx_ring,
// //         BATCH_SIZE, descs); if (rcvd > 0) {
// //             packet_count += rcvd;
// //             xsk_ring_cons__release(&umem_cfg.rx_ring, rcvd);
// //         }
// //     }

// //     printf("Total packets received: %lu\n", packet_count);

// //     // Clean up
// //     xsk_socket__delete(xsk);
// //     xsk_umem__delete(umem);
// //     free(umem_area);

// //     return 0;
// // }

// // int sock;
// // void handle_sigint(int sig) {
// //   close(sock);
// //   printf("\nServer socket closed. Exiting.\n");
// //   exit(0);
// // }
