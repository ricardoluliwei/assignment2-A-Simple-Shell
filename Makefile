# University of California, Irvine
# ICS 53
# Author: Liwei Lu
# This Makefile is able to compile all c file in the src folder
# and put the ELF file in the build folder.

# Use gcc as a compiler
CC=gcc
CFLAGS=-g
SOURCES := $(wildcard src/*.c)
EXECUTABLES := $(patsubst src/%.c, build/%, $(SOURCES))


.PHONY: all clean 
.PRECIOUS: build/% 

all: $(EXECUTABLES)
	@echo Done!

%: build/%
	@echo Build $* 

build/%: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) $^ -o $@

clean:
	@rm -rf build
	@rm -rf $*.gcda $*.gcno core.$*
	@echo Clean All!

clean-%:
	@rm -rf build/$*
	@rm -rf $*.gcda $*.gcno
	@echo Clean 
