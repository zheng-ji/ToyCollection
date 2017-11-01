#!/usr/bin/env python
# encoding: utf-8

import struct


_DELTA = 0x9e3779b9


def byteToUint32(b):
    u = []
    v = struct.unpack('>%dB' % len(b), b)
    loop = int(len(b) / 4)
    for i in range(0, loop):
        a = int(v[i * 4]) | leftShift32(int(v[i * 4 + 1]),  8) \
            | leftShift32(int(v[i * 4 + 2]), 16) | leftShift32(int(v[i * 4 + 3]), 24)
        u.append(int(a))
    return u


def uint32Tobyte(u):
    loop = int(len(u))
    b = [0] * loop * 4
    for i in range(0, loop):
        s = u[i]
        b[i*4] = s % 256
        b[i*4+1] = (s >> 8) % 256
        b[i*4+2] = (s >> 16) % 256
        b[i*4+3] = (s >> 24) % 256
    bc = ""
    for x in b:
        bc += chr(x)
    return bc


def leftShift32(x, y):
    return (x << y) % 4294967296


def btea(v, key, encrypt):
    y = 0
    z = 0
    sum = 0
    p = 0
    rounds = 0
    e = 0
    mx = 0
    n = len(v)
    if n < 2:
        return False

    if encrypt:
        rounds = int(6 + 52 / n)
        sum = 0
        z = v[n-1]

        while rounds > 0:
            sum += _DELTA
            e = (sum >> 2) & 3
            for p in range(0, n-1):
                y = v[p+1]
                mx = (((z>>5 ^ leftShift32(y, 2)) + (y>>3 ^ leftShift32(z, 4))) ^ ((sum ^ y) + (key[(p&3)^e] ^ z)))
                mx %= 4294967296
                v[p] = (v[p]+ mx) % 4294967296
                z = v[p]
            y = v[0]
            p = n-1
            mx = (((z>>5 ^ leftShift32(y, 2)) + (y>>3 ^ leftShift32(z, 4))) ^ ((sum ^ y) + (key[(p&3)^e] ^ z)))
            mx %= 4294967296
            v[n-1] = (v[n-1]+ mx) % 4294967296
            z = v[n-1]
            rounds -= 1
    else:
        rounds = int(6 + 52 / n)
        sum = rounds * _DELTA
        y = v[0]
        z
        while sum != 0:
            e = (sum >> 2) & 3
            for pp in range(0, n - 1):
                p = n - 1 - pp
                z = v[p-1]
                mx = (((z>>5 ^ leftShift32(y, 2)) + (y>>3 ^ leftShift32(z, 4))) ^ ((sum ^ y) + (key[(p&3)^e] ^ z)))
                mx %= 4294967296
                v[p] = (v[p] - mx + 4294967296) % 4294967296
                y = v[p]
            p = 0
            z = v[n-1]
            mx = (((z>>5 ^ leftShift32(y, 2)) + (y>>3 ^ leftShift32(z, 4))) ^ ((sum ^ y) + (key[(p&3)^e] ^ z)))
            mx %= 4294967296
            v[0] = (v[0] - mx + 4294967296) % 4294967296
            y = v[0]
            sum -= _DELTA
    return v


def encrypt(src, key):
    n = len(src)
    if n % 4 != 0:
        return False

    v = byteToUint32(src)
    u = btea(v, key, True)
    if u is False:
        return False
    return uint32Tobyte(u)


def decrypt(src, key):
    n = len(src)
    if n % 4 != 0:
        return False

    v = byteToUint32(src)
    u = btea(v, key, False)

    if u is False:
        return False

    return uint32Tobyte(u)
