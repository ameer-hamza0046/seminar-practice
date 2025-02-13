/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <bpf/bpf_helpers.h>

SEC("xdp")
int helloXDP(struct __sk_buff *skb) {
    return XDP_REDIRECT;
}

char _license[] SEC("license") = "GPL";