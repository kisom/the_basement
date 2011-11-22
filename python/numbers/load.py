#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# author: kyle isom <coder@kyleisom.net>
# working on deciphering a possible numbers station on reddit

# standard library
import os
import re
import simplejson
import sqlite3

# dependencies
import BeautifulSoup
import requests


# database globals
DATABASE = os.path.join(os.getcwd(), 'numbers.db')
dbo = sqlite3.connect(DATABASE)

# other globals
USER = 'A1B21F8244F'

class Comment:
    def __init__(self, source, data, created):
        self.source = source
        self.data = data
        self.created = created

        comment = data.split(' ')
        self.time = comment[1]
        if len(comment) == 7:
            self.g0 = comment[2]
            self.g1 = comment[3]
            self.g2 = comment[4]
            self.g3 = comment[5]
            self.footer = comment[6]
        elif len(comment) == 6:
            self.g0 = 'NULL'
            self.g1 = comment[2]
            self.g2 = comment[3]
            self.g3 = comment[4]
            self.footer = comment[5]
        else:
            raise Exception

    def store(self, dbo):
        cur = dbo.cursor()
        query = 'select * from comments where source = ?'
        cur.execute(query, (self.source, ))
        if cur.fetchone:
            return False
        query = 'insert into comments (source, data, created, time, g0, g1, '
        query += 'g2, g3, footer) values (?, ?, ?, ?, ?, ?, ?, ?, ?)'
        cur.execute(query, (self.source, self.data, self.created, self.time, 
                            self.g0, self.g1, self.g2, self.g3, self.footer))
        return True

class Post:
    def __init__(self, source, permalink, created):
        self.source = source
        self.permalink = 'http://www.reddit.com' + permalink
        self.created = created

    def store(self, dbo):
        cur = dbo.cursor()
        query = 'insert into posts (source, permalink, created) '
        query += 'values (?, ?, ?)'
        cur.execute(query, (self.source, self.permalink, self.created))
        dbo.commit()

def load_user():
    userpage = requests.get('http://api.reddit.com/user/' + USER)
    userdata = simplejson.loads(userpage.content)['data']['children']

    comments = [ post['data'] for post in userdata
                 if 't1' == post['kind'] ]
    posts = [ post['data'] for post in userdata
              if 't3' == post['kind'] ]
    
    comments_stored = 0
    posts_stored = 0

    print '[+] loading comments...'
    for comment in comments:
        text_line = comment['body'].strip('*')
        created = comment['created_utc']
        link = re.sub(r'\s', '_', comment['link_title'])
        link = re.sub(r'[.!?]', '', link).lower()
        source = 'http://www.reddit.com/r/%s/comments/%s/%s/%s' % (
                comment['subreddit'],
                comment['link_id'][3:],
                link,
                comment['name'][3:]
        )

        stash = Comment(source, text_line, created)
        if stash.store(dbo):
            comments_stored += 1

    print '\t[*] %d comments stored.' % (comments_stored, )
    print '[+] loading posts...'
    for post in posts:
        stash = Post(post['url'], post['permalink'], post['created_utc'])
        stash.store(dbo)
        posts_stored += 1

    print '\t[*] %d posts stored.' % (posts_stored, )

def load_subreddit():
    subreddit = requests.get('http://api.reddit.com/r/' + USER)
    sr = simplejson.loads(subreddit.content)

    sr = sr['data']['children']


if '__main__' == __name__:
    #load_user()

