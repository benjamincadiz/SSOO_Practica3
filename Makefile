#!/usr/bin/make -f
# -*- mode:makefile -*-

CC := g++ -std=c++11
DIREXEC := exec/
SRC := src/
RUN := ./

dirs:
	mkdir -p $(DIREXEC)

CINE: dirs
	$(CC) $(SRC)manager.cpp -o $(DIREXEC)CINE -pthread

all: CINE

test: CINE
	$(RUN)$(DIREXEC)CINE

clean:
	rm -rf $(DIREXEC)


