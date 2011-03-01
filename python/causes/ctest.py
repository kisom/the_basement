#!/usr/bin/env python
import causes

wordlist = open('word.list').read().split()
fl = causes.get_friend_list('aardvaark', wordlist)
print "friends: ", len(fl)

