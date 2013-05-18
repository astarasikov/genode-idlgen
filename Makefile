APPNAME=idlgen
CXX=g++

IDLGEN_TOPDIR=$(shell pwd)
CXXFLAGS=-std=c++11 -I$(IDLGEN_TOPDIR)/include

CXXFILES = idlgen.cc

OBJFILES = $(patsubst %.cc,%.o,$(CXXFILES))

all: $(APPNAME)

$(APPNAME): $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJFILES)

$(OBJFILES): %.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(APPNAME)
	rm -f *.o
