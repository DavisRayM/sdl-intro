CC=clang
CFLAGS=-std=c23 -pedantic -Wall -Werror -Wno-unused-parameter $(shell pkg-config --cflags sdl3 sdl3-image)
LDFLAGS=$(shell pkg-config --libs sdl3 sdl3-image)

.PHONY: clean

render: render.o

clean:
	rm -rf *.o render
