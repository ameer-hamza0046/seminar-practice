/* SPDX-License-Identifier: GPL-2.0 */
#include <errno.h>
#include <linux/if_link.h>
#include <linux/if_xdp.h>
#include <net/if.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define IFACE "wlo1"

int sock;
void handle_sigint(int sig) {
  close(sock);
  printf("\nServer socket closed. Exiting.\n");
  exit(0);
}

int main() {
  signal(SIGINT, handle_sigint);
  sock = socket(AF_XDP, SOCK_RAW, 0);
  if (sock < 0) {
    perror("socket");
    return 1;
  }

  struct sockaddr_xdp sxdp = {0};
  sxdp.sxdp_family = AF_XDP;
  sxdp.sxdp_ifindex = if_nametoindex(IFACE);

  if (bind(sock, (struct sockaddr *)&sxdp, sizeof(sxdp)) < 0) {
    perror("bind");
    close(sock);
    return 1;
  }

  printf("AF_XDP socket bound to %s, waiting for packets...\n", IFACE);

  char buf[2048];
  while (1) {
    int len = recv(sock, buf, sizeof(buf), 0);
    if (len > 0) {
      printf("Received %d bytes\n", len);
    }
  }

  close(sock);
  return 0;
}
