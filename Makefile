NAME = hello_af_xdp_kern
DEV = wlo1
USERPROG = hello_af_xdp

# -I/usr/include/$(shell uname -m)-linux-gnu \

$(NAME).bpf.o: %.o: %.c
	clang -target bpf \
	-g \
	-O2 -c $< -o $@
	ip link set dev $(DEV) xdp obj $(NAME).bpf.o sec xdp

$(USERPROG).o: $(USERPROG).c
	clang $(USERPROG).c -o $(USERPROG).o -lxdp

clean:
	ip link set dev $(DEV) xdp off
	rm *.o
