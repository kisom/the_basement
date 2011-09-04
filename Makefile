# BASEMENT MAKEFILE
CFLAGS := -Wall -Werror -g
CC=gcc
COMPILED := xw defscan srm srm2
PROGLIST := cproj.py $(COMPILED)
PREFIX := ${HOME}/bin

all: $(COMPILED)

defscan: defscan.c
	$(CC) $(CFLAGS) -lpcre -o defscan defscan.c

srm: srm.c
	$(CC) -Wall -o srm srm.c -lbsd

srm2: srm2.c
	$(CC) -Wall -g -o srm2 srm2.c -lbsd
	cp os wipe1
	cp os wipe2
	cp os wipe3

xw: xw.c
	$(CC) $(CFLAGS) -o xw xw.c

clean:
	rm -f *.o a.out ctags etags $(COMPILED) .*.swp *.pyc wipe?

install: $(PROGLIST)
	cp $(PROGLIST) $(PREFIX) 

uninstall: 
	for p in ${PROGLIST}; do rm -f $(PREFIX)/$$p; done

ctags: 
	ctags *.c *.h >tags

etags:
	etags *.c *.h >tags

.PHONY: clean install uninstall ctags etags
