#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author: kyle isom <coder@kyleisom.net>
#
# generate lorem ipsum testing foo bar

import getopt
import os
import random
import sys

wordlist = [ 'foo', 'bar', 'baz', 'quux', 'spam', 'eggs' ]

def make_sentence( ):
    words = random.randint(5, 15)
    
    sentence = ' '.join([ random.choice(wordlist) for i in range(words) ])
    sentence = sentence.strip()
    sentence += '.'
    return sentence

def make_paragraph( ):
    num = random.randint(5, 20)
    paragraph = ' '.join([ make_sentence() for i in range(num) ])
    return paragraph
    
def make_foobar(paragraphs):
    if None == paragraphs:
        paragraphs = random.randint(3, 10)

    foobar = '\n\n'.join([ make_paragraph() for i in range(paragraphs) ])
    foobar += '\n\n'

    return foobar

def main(args):
    paragraphs = None
    opts, args = getopt.getopt(args, 'p:')

    for (opt, arg) in opts:
        opt = opt.strip('-')
        if 'p' == opt:
            paragraphs = int(arg)
    
    if not args:
        out = sys.stdout
    else:
        out = open(args[0], 'w')

    out.write(make_foobar(paragraphs))

if '__main__' == __name__:
    main(sys.argv[1:])

