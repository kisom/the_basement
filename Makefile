# BASEMENT MAKEFILE
CFLAGS := -Wall -Werror -g
CC=gcc
PROGLIST := xw defscan
PREFIX := ${HOME}/bin

defscan: defscan.c
	$(CC) $(CFLAGS) -o defscan defscan.c

xw: xw.c
	$(CC) $(CFLAGS) -o xw xw.c

clean:
	rm -f *.o a.out ctags etags $(PROGLIST) .*.swp

install: $(PROGLIST)
	cp $(PROGLIST) $(PREFIX) 

ctags: 
	ctags *.c *.h >tags

etags:
	etags *.c *.h >tags
