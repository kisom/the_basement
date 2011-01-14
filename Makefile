# BASEMENT MAKEFILE
CFLAGS := -Wall -Werror -g
CC=gcc
PROGLIST := xw
PREFIX := ${HOME}/bin

xw: xw.c
	$(CC) $(CFLAGS) -o xw xw.c

clean:
	rm -f *.o a.out $(PROGLIST) .*.swp

install:
	cp $(PROGLIST) $(PREFIX)
