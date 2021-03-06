# BASEMENT MAKEFILE
CFLAGS := -Wall -Werror -g
CC=gcc
COMPILED := xw defscan srm irclogd
PROGLIST := cproj.py $(COMPILED)
PREFIX := ${HOME}/bin

all: $(COMPILED)

defscan: defscan.c
	$(CC) $(CFLAGS) -lpcre -o defscan defscan.c

irclogd: irclogd.c
	$(CC) $(CFLAGS) -o irclogd irclogd.c

srm: srm.c
	$(CC) $(CFLAGS) -o srm srm.c

xw: xw.c
	$(CC) $(CFLAGS) -o xw xw.c

clean:
	rm -f *.o a.out tags TAGS $(COMPILED) .*.swp *.pyc wipe?

install: $(PROGLIST)
	cp $(PROGLIST) $(PREFIX) 

uninstall: 
	for p in ${PROGLIST}; do rm -f $(PREFIX)/$$p; done

wipetarget:
	cp os wipe1
	cp os wipe2
	cp os wipe3

ctags: 
	ctags *.c *.h >tags

etags:
	etags *.c *.h >TAGS

.PHONY: clean install uninstall ctags etags wipetarget
