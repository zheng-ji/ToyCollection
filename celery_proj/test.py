#!/usr/bin/env python
# encoding: utf-8

from apps.tasks import add
import gevent


concurrent = 300
limit = 900
if __name__ == '__main__':


    for x in range(100):
        print add.apply_async(args=(3, 4, ), queue='test')

    print "over"
