CC=gcc
CFLAGS=-Wall -Wextra -Iinclude

SRC=$(wildcard src/*.c)

mini_mux:
	$(CC) $(CFLAGS) $(SRC) -o mini_mux

clean:
	rm -rf .cache/ \
	rm mini_mux

all: mini_mux
