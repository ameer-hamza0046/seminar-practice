#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/ptrace.h>
#include <linux/bpf_common.h>
#include <bpf/bpf_helpers.h>

// error: legacy map definitions in 'maps' section are not supported by libbpf v1.0+
// libbpf: prog 'packet_counter': bad map relo against 'packet_count' in section 'maps/packet_count'

#define MAP_SIZE 1024

struct bpf_map_def {
	unsigned int type;
	unsigned int key_size;
	unsigned int value_size;
	unsigned int max_entries;
	unsigned int map_flags;
};

struct bpf_map_def SEC("maps/packet_count") packet_count = {
    .type = BPF_MAP_TYPE_HASH,
    .key_size = sizeof(int),
    .value_size = sizeof(long),
    .max_entries = MAP_SIZE,
};

SEC("xdp")
int packet_counter(struct xdp_md* ctx) {
    int key = 0;
    long *value;
    value = bpf_map_lookup_elem(&packet_count, &key);
    if (value) {
        (*value)++;
    } else {
        long initial_value = 1;
        bpf_map_update_elem(&packet_count, &key, &initial_value, BPF_ANY);
    }
    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
