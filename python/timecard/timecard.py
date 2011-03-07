#!/usr/bin/env python

import datetime
import getopt
import os
import re
import sys

import pdb

logfile         = os.path.join(os.getenv('HOME'), 'timecard.log')
project         = None
clocked         = None              # True if clocked in

def get_datetime(timestamp):
    """
    Returns a datetime object from str(datetime.datetime).
    """


    ctor        = []
    timestamp   = timestamp.split()
    date        = [ int(i) for i in timestamp[0].split('-') ]
    time        = [ int(i) for i in timestamp[1].replace('.', ':').split(':') ]

    ctor.extend(date)
    ctor.extend(time)
    return eval('datetime.datetime(%d, %d, %d, %d, %d, %d, %d)' % tuple(ctor))

def write_log():
    """
    Logs user as clocking in or out.
    """
    now         = datetime.datetime.now()

    f           = get_log(write = True)

    if clocked:
        state   = 'in'
    else:
        state   = 'out'

    print >>f, 'clocked %s on project %s at %s' % (state, project, str(datetime.datetime.now()))
    f.close()

    return True

def get_log(write = False):
    """
    Returns the logfile as an array of strings representing lines in the logfile.
    """

    try:
        f       = open(logfile, 'a+')
    except IOError, e:
        print e
        return False
    except:
        return False

    if not write:
        return [ line for line in f.read().split('\n') if line ] 
    else:
        return f

def check_state():
    """
    Sets clocked state.
    """

    global clocked

    log = get_log()
    assert(not False == log)

    if len(log) == 0:
        clocked = False
    else:
        clocked = not len(log) % 2 == 0

def get_project():
    """
    Sets the global project name variable. This is only useful if clocked in.
    If not clocked in, it will return None.
    """

    global project

    if not clocked:
        return None
    
def get_state():
    if None == clocked:
        check_state()
    return clocked

def clock():
    """
    Clocks the user in and out.
    """
    check_state()
    assert(not clocked   == None)
    assert(write_log()   == True)

def get_last_project():
    """
    Returns the last project worked on.
    """
    log = get_log()
    log = log[-1]

    project = None

    if not None == log:
        project = re.search('on project ([\w\s]+) at ', log).group(1)
    
    return project

def main(command = None):
    if 'clock' == command:
        clock()
    elif 'status' == command:
        if get_state():
            print 'You are clocked in on project %s.' % project
        else:
            print 'You are not clocked in.'
    elif None == command:
        clock()
    else:
        print 'invalid command!'
        usage()
    sys.exit(0)

if __name__ == '__main__':
    #pdb.set_trace()
    
    if not len(sys.argv) > 1:
        project = get_last_project()
        main()
    else:
        (opts, args) = getopt.getopt(sys.argv[1:], 'h', [ 'report=', 'time', 'log' ])
        cmd          = None

        if not opts:
            if '?' in args:
                main('status')
            else:
                project = args[-1]
                main()

        for (opt, arg) in opts:
            if opt == 'log': 
                cmd = 'log'
            if opt == 'report':
                cmd = '%s %s' % ('report', arg)
            if opt == 'h':
                usage()


        main(cmd)
