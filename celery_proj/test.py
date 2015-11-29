#!/usr/bin/env python
# encoding: utf-8

from apps.tasks import add

if __name__ == '__main__':
    print add(2, 3)
    #print add.delay(3, 4)
