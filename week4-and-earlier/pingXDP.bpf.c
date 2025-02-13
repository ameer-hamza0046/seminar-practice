/* SPDX-License-Identifier: GPL-2.0 */
#include "vmlinux.h"
#include <bpf/bpf_endian.h>
#include <bpf/bpf_helpers.h>
#include "packet.h"

SEC("xdp")
int pingXDP(struct xdp_md* ctx) {
    long protocol = lookup_protocol(ctx);
    bpf_printk("Packet received: \n");
    if(protocol == 1) {
        bpf_printk("\tHello ping\n");
    }
    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";