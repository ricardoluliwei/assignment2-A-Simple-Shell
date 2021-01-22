<<<<<<< HEAD
# University of California, Irvine
# ICS 53
# Author: Liwei Lu
# This Makefile is able to compile all c file in the src folder
# and put the ELF file in the build folder.

# Use gcc as a compiler
CC=gcc
CFLAGS=-g -fprofile-arcs -ftest-coverage
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
	@rm -rf $(filter-out Makefile src, $(wildcard *))
	@echo Clean All!

clean-%:
	@rm -rf build/$*
	@rm -rf $*.gcda $*.gcno
	@echo Clean 
=======
all:
	@gcc src/sh.c -o shell
	@echo Compile Completed, Start Running
	@./shell

clean:
	@rm shell
	@echo Clean Completed
>>>>>>> Qiwei_He
