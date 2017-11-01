# -*- coding: utf-8 -*-

import time
import random
import struct
from . import xxtea
from hashlib import md5

def byteToKey(b):
    u = [0, 0, 0, 0]

    if len(b) != 16:
        return u
    
    v = struct.unpack('>%dB' % len(b), b)
    for i in range(0, 4):
        u[i] = int(v[i * 4]) | xxtea.leftShift32(int(v[i * 4 + 1]), 8) \
            | xxtea.leftShift32(int(v[i * 4 + 2]), 16) \
            | xxtea.leftShift32(int(v[i * 4 + 3]), 24)
    return u


def mpEncode(src, key):
    m = md5()
    m.update(key)
    new_key = byteToKey(m.digest())

    t = int(time.time())
    size = len(src)
    padding = 4 - size % 4
    total_len = (8 + size + padding)
    head = chr(random.randint(0, 255) & 0xf8 | (padding & 7))

    tmp = struct.pack('>c', head)
    format_tmp = '>%ds' % padding
    tmp += struct.pack(format_tmp, "")
    format_tmp = '>%dsI3s' % len(src)
    tmp += struct.pack(format_tmp, src, t, "")
    if len(tmp) != total_len:
        return False
    return xxtea.encrypt(tmp, new_key)


def mpDecode(src, key):
    m = md5()
    m.update(key)
    key = byteToKey(m.digest())

    size = len(src)
    if size % 4 != 0 or size < 12:
        return False

    tmp = xxtea.decrypt(src, key)
    if tmp is False:
        return False

    padding = int(tmp[0].encode('hex'), 16) & 7
    if padding > 4:
        return False

    for p in range(0, padding):
        if tmp[p+1].encode('hex') != '00':
            return False

    for p in range(1, 4):
        if tmp[size-p].encode('hex') != '00':
            print(3)
            return False

    return tmp[padding+1: size-7]
