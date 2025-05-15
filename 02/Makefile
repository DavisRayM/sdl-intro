CC=clang
CFLAGS=-std=c23 -pedantic -Wall -Werror -Wno-unused-parameter $(shell pkg-config --cflags sdl3)
LDFLAGS=$(shell pkg-config --libs sdl3)

.PHONY: clean

image: image.o

clean:
	rm -f *.o image
