NAME = xdp_redirect

$(NAME).bpf.o: %.o: %.c
	clang -I/usr/src/linux-headers-6.8.0-49-generic/tools/bpf/resolve_btfids/libbpf/include/ \
	-target bpf \
	-I/usr/include/$(shell uname -m)-linux-gnu \
	-g \
	-O2 -c $< -o $@
	ip link set dev wlo1 xdp obj $(NAME).bpf.o sec xdp

helloAF_XDP.o: helloAF_XDP.c
	gcc helloAF_XDP.c -o helloAF_XDP.o

clean:
	ip link set dev wlo1 xdp off
	rm *.o