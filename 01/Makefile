CC=clang
CFLAGS=-std=c23 -pedantic -Wall -Wextra -Werror -Wno-unused-parameter $(shell pkg-config --cflags sdl3)
LDFLAGS=$(shell pkg-config --libs sdl3)

.PHONY: clean

sample: sample.o

clean:
	rm -f sample.o sample
