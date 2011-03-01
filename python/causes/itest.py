#!/usr/bin/env python
# interactive test

from lev import lev_dist

string_a = "NOTNULL"
string_b = "NOTNULL"

while True:
    string_a = raw_input("string A: ").strip() 
    string_b = raw_input("string B: ").strip()

    if not string_a or not string_b: break
    
    print "the lev dist between '%s' and '%s' is %d" % (string_a, string_b,
        lev_dist(string_a, string_b))

