CC=gcc
CFLAGS=-I
CFLAGS+=-Wall
FILES= mine.c utils.c

mine: $(FILES)
	$(CC) $(CFLAGS) $^ -o $@ 

clean:
	rm -r *.o mine

all: mine