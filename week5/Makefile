NAME = hello_af_xdp_kern
DEV = wlo1
USERPROG = hello_af_xdp

# -I/usr/include/$(shell uname -m)-linux-gnu \

$(USERPROG).o: $(USERPROG).c
	clang $(USERPROG).c -o $(USERPROG).o -lxdp

$(NAME).bpf.o: %.o: %.c
	clang -target bpf \
	-g \
	-O2 -c $< -o $@
	ip link set dev $(DEV) xdp obj $(NAME).bpf.o sec xdp

clean:
	ip link set dev $(DEV) xdp off
	rm *.o
