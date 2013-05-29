
MAKEFILE_IN = $(PWD)/Makefile.in
include $(MAKEFILE_IN)

CFLAGS   ?= -Wall

LOCLIBS = src/libcalvis.a cow/libcow.a

ifeq ($(strip $(USE_HDF5)), 1)
MODULES += $(LUA_HDF5)
DEFINES += -DUSE_HDF5
LIBS += -L$(HDF_HOME)/lib -lz -lhdf5
endif

LIBS += -framework OpenGL -framework GLUT

default : bin/calvis

bin/calvis : $(LOCLIBS)
	@mkdir -p bin
	$(CXX) $(CFLAGS) -o $@ $^ $(LIBS)

src/libcalvis.a : .FORCE
	$(MAKE) -C src

cow/libcow.a : .FORCE
	$(MAKE) -C cow libcow.a MAKEFILE_IN=$(MAKEFILE_IN)

clean :
	$(MAKE) -C cow clean MAKEFILE_IN=$(MAKEFILE_IN)
	$(MAKE) -C src clean MAKEFILE_IN=$(MAKEFILE_IN)
	$(RM) bin/calvis

.FORCE :
