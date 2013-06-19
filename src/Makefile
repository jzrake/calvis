
ARSTATIC ?= $(AR) rcu
RANLIB ?= ranlib

CXXFLAGS ?= -Wall -O2

CXX_SRC = $(wildcard *.cpp)
CXX_OBJ = $(CXX_SRC:.cpp=.o)

default : libcalvis.a
show :
	@echo "src files: $(CXX_SRC)"
	@echo "obj files: $(CXX_OBJ)"
libcalvis.a : $(CXX_OBJ)
	$(ARSTATIC) $@ $?
	$(RANLIB) $@
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^ -I/usr/X11R6/include -I../cow
clean :
	$(RM) *.o libcalvis.a

