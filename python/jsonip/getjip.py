#!/usr/bin/env python
"""
just a quick IP grabber using jsonip.com.
"""

import json
import urllib2

print json.loads(urllib2.urlopen('http://www.jsonip.com').read())['ip']

