#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
int counter = 0;
SEC("kprobe/sys_execve")

int count_exec(void *ctx) {
    counter++;
    bpf_printk("sys_execve called: %d", counter);
    return XDP_PASS;
}
char LICENSE[] SEC("license") = "Dual BSD/GPL";
