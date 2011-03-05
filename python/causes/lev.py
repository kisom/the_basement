#!/usr/bin/env python
# calculate levenshtein distance for two tokens

# instead of just a list comprehension... 
class Matrix():
    rows    = None
    cols    = None
    m       = None

    def __init__(self, x, y):
        self.rows = x
        self.cols = y 

        self.matricks = [ [0] * self.cols for i in range(self.rows) ]

    def set(self, x, y, value):
        self.matricks[x][y] = value
    
    def get(self, x, y):
        return self.matricks[x][y]

    def display(self):
        rows = [ i for i in range(self.cols) ]
        print ' ', rows
        for i in range(self.rows):
            print i, self.matricks[i]
       

# because levenshtein distance doesn't roll off the tongue quite as easily
def lev_dist(string_a, string_b):
    
    mx = None
    
    # step 1
    n = len(string_a)   # cols
    m = len(string_b)   # rows

    if not n or not m:
        return 0

    mx = Matrix(m + 1, n + 1)

    # step 2
    for i in range(n + 1):
        mx.set(0, i, i)

    for i in range(m + 1):
        mx.set(i, 0, i)


    # step 3-6
    for y in range(1, n + 1):
        for x in range(1, m + 1):
            
            if string_a[y - 1] == string_b[x - 1]:
                cost = 0
            else:
                cost = 1
            
            cell = min(
                mx.get(x - 1, y) + cost,
                mx.get(x, y - 1) + cost,
                mx.get(x - 1, y - 1) + cost
            )

            mx.set(x, y, cell)

    return mx.get(m, n)

