#!/usr/bin/env python

from lev import lev_dist

def are_friends_p(me, you):
    """
Predicate checking friend status; friend is designated as the fact that the
levenshtein distance between two words is 1. Some performance-enhancing
drugs (logic) are used to prevent the use of the 'expensive' levenshtein
algorithm.
    """

    # don't include duplicates
    if me == you or me is you:
        print "EQL"
        return False
    # more than 2 char difference means it is impossible to get an LD < 2
    elif (abs(len(me) - len(you)) > 2):
        print "ABS"
        return False
    elif lev_dist(me, you) > 1:
        print "LEV"
        return False
    else:
        return True


def get_friend_list(word, words):
    # friends = [ w for w in words if not 
    friends = [ w for w in words if are_friends_p(word, w) ]

    return friends


