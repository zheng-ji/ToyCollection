#!/usr/bin/env python
# encoding: utf-8

from apps.tasks import add
import gevent


concurrent = 300
limit = 900
if __name__ == '__main__':

    def work():
        print add.apply_async(args=(3, 4, ), queue='test')

    count = 0
    while True:
        threads = []
        for i in range(concurrent):
            threads.append(gevent.spawn(work))
        gevent.joinall(threads)
        count += concurrent
        if limit is not None and count >= limit:
            break
    print "over"
