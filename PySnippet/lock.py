#!/usr/bin/env python
# encoding: utf-8

class Lock(object):
    def __init__(self, key):
        self._key = key
        self.redis_client = redis
        self._get_lock = False

    def lock(self):
        if self.redis.setnx(self._key, 1):
            self._get_lock = True
            self.redis.expire(self._key, 20)
            return True
        else:
            return False

    def unlock(self):
        self.redis.delete(self._key)

    def __enter__(self):
        return self.lock()

    def __exit__(self, exc_type, exc_value, exc_traceback):
        if self._get_lock:
            self.unlock()


def check_lock(key):
    if redis_client.setnx(key, 1):
        ttl = 20
        redis_client.expire(key, 20)
        return True
    else:
        return False

def release_lock(key):
    redis_client.delete(key)

key = "scaleup_lock"
if check_lock("scaleup_lock"):
    # TODO LOGIC
    release_lock("scaleup_lock")
else:
    return 
