#!/usr/bin/env python
"""
just a quick IP grabber using jsonip.com.
"""

import json
from urllib2 import urlopen

print json.loads(urlopen('http://www.jsonip.com').read())['ip']

