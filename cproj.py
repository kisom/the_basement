#!/usr/bin/env python
# cproj.py:
#   tools to facilitate creating new C projects
#   

import datetime
import getopt
import os
import re
import sys

config  = {
    'nameline':'4096R/B7B720D6  "Kyle Isom <coder@kyleisom.net>"',
    'datefmt':'%Y-%m-%d',
    'border': { 'left':1, 'right':75 },
    'basedir':None,
    'basename':None,
    'lang':'C',
}

HEADER_EMPTYLINE = ' *' + ' ' * 72 + '*\n'
HEADER_TEMPLATE  = '/' + '*' * 74 + '\n'
HEADER_TEMPLATE += ' * %s.%s%s*\n'          # basename, extension, padding
HEADER_TEMPLATE += ' * %s%s*\n'             # author line, padding
HEADER_TEMPLATE += ' * %s%s*\n'             # date, padding
HEADER_TEMPLATE += HEADER_EMPTYLINE
# end basic header building

def usage( ):
    print "usage: %s <options> [command]" % sys.argv[0]
    print """commands: 
    project:    create a new project (src dir and all)
    newsrc:     create a new .c file with .h and headers
    
    """
    sys.exit(0)


def main(func = None):
    
    sys.exit(0)

def pop_header(basename, extensions):
    """
    populates the header - formatting code goes here
    """
    header  = {}
    descr   = ""
    line    = " "
    pad     = config['border']['right'] - config['border']['left'] - 1

    for extension in extensions:
        header[extension]  = HEADER_TEMPLATE
        # build line 1
        filenam = '%s.%s' % (basename, extension)
        basepn  = pad - len(filenam) - 2
        basepad = ' ' * basepn

        authpn  = pad - len(config['nameline']) - 2
        authpad = ' ' * authpn

        today   = datetime.date.strftime(datetime.date.today(), '%Y-%m-%d')
        dtspn   = pad - len(today) - 2
        dtspad  = ' ' * dtspn

        header[extension]  = header[extension] % (basename, extension, basepad, 
                             config['nameline'], authpad, today, dtspad)

    # build summary
    summary = raw_input('one line summary of files: ')
    sumpad  = pad - len(summary) - 2
    summary = ' * %s' % summary
    summary += ' ' * sumpad + '*\n'

    for extension in extensions:
        header[extension] += summary
        header[extension] += HEADER_EMPTYLINE

    print ' ' + '*' * 74
    print "enter DESCR, empty line ends (long lines will be truncated!): "
    print "(protip - the astericks are the ruler, don't go past them)"
    while (not line == ""):
        # finangle the header into happiness!
        # Citizen Header: comply or face the consequences!
        try:
            line = raw_input('> ')
        except EOFError:
            break
        line = line.replace('\t', ' ' * 8)  # tab->whitesp
        line = line[:70]
        descr += ' * %s' % line     # need to use config['border']['left'] here
        lipad  = pad - len(line) - 2
        descr += ' ' * lipad
        descr += '*\n'

    for extension in extensions:
        header[extension] += descr
        header[extension] += ' ' + '*' * 74 + '/\n\n'

    return header

def new_csrc(basename = 'main'):
    ext = [ 'c', 'h' ]
    headers = pop_header(config['basename'], ext)

    for e in ext:
        try:
            f = open('%s.%s' % (basename, e), 'w+')
        except IOError, e:
            print e
            sys.exit(1)
        else:
            f.write(headers[e])
            f.close()

    return True       

def new_src( ):
    if not config['basedir']:
        config['basedir'] = './'
    if not config['basename']:
        config['basename'] = raw_input('source file name: ')

    if not config['basedir'][-1] == '/':
        config['basedir'] += '/'

    basename = '%s%s' % (config['basedir'], config['basename'])

    if 'C' == config['lang']:
       new_csrc(basename) 

    return True

def sel_cmd(command):

    if 'newsrc' == command:
        new_src( )

    return None

##### __main__ #####

if '__main__' == __name__:
    (opts, args) = getopt.getopt(sys.argv[1:], 'hda')
    cmds         = [ 'newsrc', 'project' ]
    
    # check to make sure our commands work
    for cmd in cmds:
        if cmd == args[0]: break
    else:
        print "invalid command!"
        sys.exit(1)

    for opt in opts:
        if 'h' in opt[0]:
            usage()

        if 'd' in opt[0]:
            config['basedir'] = opt[1]
    
        if 'a' in opt[0]:
            config['nameline'] = opt[1]

    if 2 == len(args):
        config['basename'] = args[1]

    sel_cmd(args[0])
