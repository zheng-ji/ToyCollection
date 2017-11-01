#! /usr/bin/env python
# -*- coding: utf-8 -*-
import os
import config
import pickle as pickle

from flask.globals import current_app


def get_ext(filename):
    """获取文件名后缀（小写），如: .jpg"""
    return os.path.splitext(filename)[1].lower()


def cached(cache_time, cache_name='DEFAULT'):
    def middle(func):
        def wrap(*args, **kargs):
            redis_cache = current_app.extensions['redis_cache']

            module = func.__module__.split('.')[1]
            cache_key = "%s.%s?%s" % (module, func.__name__, ','.join([str(r) for r in args]))
            force_reload = kargs.get('force_reload', None)
            if force_reload is not None:
                del kargs['force_reload']

            cache_key += ','.join([str(r[1]) for r in list(kargs.items())])
            data = redis_cache[cache_name].get(cache_key)
            if data and not force_reload and not config.FORCE_RELOAD:
                return pickle.loads(data)
            else:
                data = func(*args, **kargs)
                if data:
                    redis_cache[cache_name].set(cache_key, pickle.dumps(data), cache_time)
                return data
        return wrap
    return middle


class Lock():

    def __init__(self, key):
        redis_cache = current_app.extensions['redis_cache']
        self._key = key
        self._redis = redis_cache['LOCK']
        self._get_lock = False

    def lock(self):
        """此处进程会阻塞， 考虑用异步切换进程"""
        if self._redis.setnx(self._key, 1):
            self._get_lock = True
            self._redis.expire(self._key, 20)
            return True
        else:
            return False

    def unlock(self):
        self._redis.delete(self._key)

    def __enter__(self):
        return self.lock()

    def __exit__(self, exc_type, exc_value, exc_traceback):
        if self._get_lock:
            self.unlock()


def list_to_dict(src_list, key):
    """
        将一个对象列表转换成对象字典，

        @input:
            src_list: 对象列表
            key: 对象的一个属性的key，可以通过 getattr(obj, key) 获得熟悉取值
    """

    d = {}
    for item in src_list:
        k = getattr(item, key)
        d[k] = item

    return d
