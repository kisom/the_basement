#!/usr/bin/env python
# reference speed test for searching wordlist

import sys

wordlist    = open('word.list').read().split()
found       = False
counter     = 0

for word in wordlist:
    if word == sys.argv[1]: found = True
    counter += 1

if found: print 'found!'
else: print 'not found!'

print counter


