CC=gcc
CFLAGS=-I
CFLAGS+=-Wall
DEBUG=0
DEBUGFLAG=-DDEBUG
FILES= mine.c utils.c

mine: $(FILES)
ifeq ($(DEBUG), 1)
	$(CC) $(DEBUGFLAG) $(CFLAGS) $^ -o $@ 
else
	$(CC) $(CFLAGS) $^ -o $@ 
endif

clean:
	rm -r *.o mine

all: mine