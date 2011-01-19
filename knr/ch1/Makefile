CFLAGS := -Wall -Werror
CC := gcc
PROGRAMS := detab

all: $(PROGRAMS)

detab: detab.c
	$(CC) $(CFLAGS) -o detab detab.c
	
clean:
	rm -rf $(PROGRAMS) tags TAGS

ctags:
	ctags *.[ch] >tags

etags:
	etags *.[ch] >TAGS

tags: ctags etags

.PHONY: ctags etags all clean 
