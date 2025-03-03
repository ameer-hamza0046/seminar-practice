DEV1 = wlo1
DEV2 = lo
USERPROG = hello
CC = clang

$(USERPROG).o: $(USERPROG).c
	$(CC) $(USERPROG).c -o $(USERPROG).o -lxdp

clean:
	ip link set dev $(DEV1) xdp off
	ip link set dev $(DEV2) xdp off
	rm *.o
