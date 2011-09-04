# testing producer / consumer models for multiprocessing

import multiprocessing

target      = 0
test_f      = None

def producer(maxval, zero_start = False):
    for i in range(0 if zero_start else 1, maxval + 1):
        yield i

def consumer(value, test_f):
    return value if test_f(value) else False

def consumer_worker(value):
    return value if test_f(value) else False

def set_consumer_test(f):
    global test_f
    
    test_f  = f

def set_target(val):
    global target
    
    target = val

def test_target(testval):
    return True if (testval == target) else False




# single threaded dispatcher to consume values from the producer
def dispatch1(maxval):
    gen     = producer(maxval)
    val     = 0
    found   = False
    
    while not found:
        try:
            val = gen.next()
            found = consumer(val, test_target)
        except StopIteration:
            print 'target not found!'
            break
    else:
        print 'value: %d' % val


    
# task out a pool of workers to consume the producer
def dispatchpool(maxval, n_workers):
    gen     = producer(maxval)
    
    pool    = multiprocessing.Pool(n_workers,
                                   set_consumer_test, (test_target, ))
    results = pool.map(consumer_worker, gen)
    
    res     = [ res for res in results if res ]
    
    for r in res:
        print r
    

    
    