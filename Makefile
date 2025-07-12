CC=gcc
CFLAGS=
LDFLAGS=-lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

SRC=main.c
OUT=pong

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT) 