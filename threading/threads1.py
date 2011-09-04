#!/usr/bin/env python

import os
import pdb
import random
import socket
import threading
import time


network_lock    = threading.Lock()
thread_list     = [ ]
site            = 'www.dukgo.com'

def do_net():
    tid = threading.current_thread().ident
    print '%d alive!' % tid
    
    while True:
        # construct socket
        try:
            print '%d connecting...' % tid
            s   = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.settimeout(5)
            s.connect((site, 80))
        except Exception, e:
            print '%d: error connecting to %s' % (tid, site)
            print '\t%s' % str(e)
            s.close()
            s = DummySocket()
        
        # transceiver
        s.send('GET HTTP/1.1\r\n')
        page = s.recv(64)
        if not page:
            page = 'no data!' + (' ' * 64 )
        try:
            print '\td: %s' % (tid, page)
        except TypeError, e:
            print str(e)
            pdb.set_trace()
        s.close()
        
        # random delay to lay with threading
        delay = random.uniform(0.01, 3.0)
        print '%d: %f' % (tid, delay)
        
        print '%d: completed iteration!' % tid
    
    
class DummySocket():
    def send(self, garbage):
        pass
    def recv(self, garbage):
        pass
    def close(self):
        pass


for i in range(0, 10):
    thread_list.append(threading.Thread(target=do_net))

for t in thread_list:
    #t.daemon = True
    t.start()

while True:
    pass

