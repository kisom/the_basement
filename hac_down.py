#!/usr/bin/env python
# -*- coding: utf-8 -*-
# file: hac_down.py
# author: kyle isom <coder@kyleisom.net>
#
# download the PDF of the handbook of applied cryptography
# note that this is one of the most invaluable crypto reference books -
#   you absolutely should have a hard copy. i used this to grab a pdf
#   copy for my tablet...

import os
import urllib2
from BeautifulSoup import BeautifulSoup as Soup

print '[+] downloading handbook of applied cryptography'

target_base = os.path.join(os.path.expanduser('~'), 'tmp', 'hac')

if not os.access( target_base, os.F_OK ):
    print '[+] creating', target_base
    os.makedirs(target_base)

print '[+] loading the main page...'
hac_url     = 'http://www.cacr.math.uwaterloo.ca/hac/'
hac_page    = urllib2.urlopen(hac_url).read()
soup        = Soup(hac_page)

print '[+] building link list...'
links       = [ hac_url + tag.attrs[0][1] 
                for tag in soup.findAll('a') 
                if 'href' in tag.attrs[0] and 
                   'pdf' in tag.contents ]
print '[+] grabbed %d links...' % len(links)

for link in links:
    target  = os.path.join(target_base, os.path.basename(link))

    try:
        f   = open(target, 'wb')
    except IOError as e:
        print e
        continue
    else:
        print '[+] downloading', link
        pdf = urllib2.urlopen(link).read()
        
        print '[+] writing', target
        f.write(pdf)

        f.close()


