#!/usr/bin/env python

import datetime
import getopt
import sys

import pdb

def get_datetime(timestamp):

    ctor        = []
    timestamp   = timestamp.split()
    date        = [ int(i) for i in timestamp[0].split('-') ]
    time        = [ int(i) for i in timestamp[1].replace('.', ':').split(':') ]

    ctor.extend(date)
    ctor.extend(time)
    print ctor
    return eval('datetime.datetime(%d, %d, %d, %d, %d, %d, %d)' % tuple(ctor))

