CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g

SRC = $(wildcard src/*.c)
# Put your real program name after bin/
OUT = bin/neolang

all:
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)

.PHONY: all clean