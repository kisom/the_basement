#!/usr/bin/env python
# calculate social network and solve the problem

from causes import get_friend_list

import sys

if len(sys.argv) < 2:
    arg = None
else:
    arg = sys.argv[1]

word_networks   = { }   # dictionary associating social network to word
my_network      = None  # my network

me              = arg or 'causes'

wordlist        = open('word.list').read().split()

for word in wordlist:
    print 'processing network for ', word
    word_networks[word] = get_friend_list(word, wordlist)

if me in wordlist:
   my_network = set(word_networks[me])

print me



