#!/usr/bin/env python
# automated lev dist test

from lev import lev_dist

print "lev dist between 'test' and 'test': ", lev_dist('test', 'test')
print "lev dist between 'gumbo' and 'gambol': ", lev_dist('gumbo', 'gambol')
