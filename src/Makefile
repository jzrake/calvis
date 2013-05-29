
ARSTATIC ?= $(AR) rcu
RANLIB ?= ranlib

default : libcalvis.a

libcalvis.a : cv.o
	$(ARSTATIC) $@ $?
	$(RANLIB) $@

cv.o : cv.cpp
	$(CXX) -c -o $@ $^ -I/usr/X11R6/include

clean :
	$(RM) *.o libcalvis.a

