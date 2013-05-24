
default : libcalvis.a

ARSTATIC ?= $(AR) rcu
RANLIB ?= ranlib

libcalvis.a : main.o gl-window.o
	$(ARSTATIC) $@ $?
	$(RANLIB) $@

gl-window.o : gl-window.c
	$(CC) $(CFLAGS) -c $^ -I../cow -I/usr/X11R6/include

main.o : main.c
	$(CC) $(CFLAGS) -c $^ -I../cow
