#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author: kyle isom <kyle@nodality.io>
# license: isc / public domain (http://www.brokenlcd.net/license.txt)
#
# very basic for-shits-and-giggles spellchecker

"""Spellchecker I wrote after reading http://prog21.dadgum.com/29.html."""

import getopt
import re
import sys


DICTIONARY = {'en_US': '/usr/share/dict/american-english',
               'en_GB': '/usr/share/dict/british-english',
             }
DEFAULT_LOCALE = 'en_GB'


def usage():
    """Standard usage message."""
    print 'usage: %s [-d dictionary] <file list>' % sys.argv[0]
    exit(1)


def get_dictionary(wordlist):
    """Load the dictionary, returns a list of valid words."""
    try:
        dictionary = [word for word in open(wordlist).read().split('\n')]
    except IOError as exception:
        print 'error opening dictionary file ' + wordlist
        print 'please check the traceback for more information.\n\n'
        raise exception
    else:
        return dictionary


def parse_word(token):
    """Parse the word out of a token, i.e. scrubbing punctuation."""
    word = re.sub(r"[^\w']", '', token)
    words = [word, word.lower()]
    return words


def check_word(word, dictionary):
    """Compare a parsed word to the dictionary."""
    # pulled out of the main spellcheck code because this causes another
    # layer of mental gymnastics when it can be black-boxed. we do this here
    # because this lets us not have to worry about proper nouns and so forth.
    for variant in parse_word(word):
        if variant in dictionary:
            return True
    return False


# keeping the wordlist default arg in so this can be used separately from
# other programs
def spellcheck(filename, wordlist=DICTIONARY[DEFAULT_LOCALE]):
    """Spellcheck the given file with the (optionally) specified dictionary."""
    dictionary = get_dictionary(wordlist)
    textf = open(filename)

    lineno = 0
    misspelt = []
    for line in textf:
        lineno += 1
        for token in line.split(' '):
            check_word(token, dictionary)
            if not check_word(token, dictionary):
                misspelt.append(tuple([parse_word(token)[0], lineno]))

    textf.close()

    print '\n*** spellcheck results for %s' % filename
    print '\t%s misspelt words.' % ('no' if not len(misspelt)
                                         else str(len(misspelt)), )

    for misspelling in misspelt:
        print '\t%s misspelt on line %d' % (misspelling[0], misspelling[1])

    return


def main():
    """This function handles all the commandline-usage setup."""
    opts, args = getopt.getopt(sys.argv[1:], 'd:l:')
    if not args:
        usage()

    wordlist = DICTIONARY[DEFAULT_LOCALE]
    for opt, val in opts:
        if '-d' == opt:
            wordlist = val
        if '-l' == opt:
            try:
                wordlist = DICTIONARY[val]
            except KeyError:
                print "%s isn't recognised as a language."
                exit(2)

    print "dictionary: %s" % (wordlist, )
    for filename in args:
        spellcheck(filename, wordlist)


if '__main__' == __name__:
    main()
