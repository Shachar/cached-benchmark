NUM_FUNCTIONS=16
FUNC_ALIGNMENT=128

CXXFLAGS=-Wall -O2 -g -std=c++20
LD=$(CXX)
CC=$(CXX)

include parameters.mk

all: benchmark1 benchmark2

benchmark: benchmark.o funcref.o

benchmark1.o: time.h

funcref.o: function.h
benchmark.o: function.h

parameters.mk funcref.cpp function.h: Makefile gen_parameters
	./gen_parameters $(NUM_FUNCTIONS) $(FUNC_ALIGNMENT)

clean:
	$(RM) *.o function_*.cpp parameters.mk benchmark function.h funcref.cpp benchmark1 benchmark2
.PHONY: clean

function_%.cpp: function.cpp
	sed -e 's:@@ALIGNMENT@@:$(FUNC_ALIGNMENT):g ; s:@@NUMBER@@:$*:g' $< > $@.tmp
	mv $@.tmp $@
