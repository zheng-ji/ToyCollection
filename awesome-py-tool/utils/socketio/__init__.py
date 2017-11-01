#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Author Hector Qiu <hectorqiuiscool@gmail.com>
# Copyright © 2016 Youmi

"""
socketio 外部消息发送工具，

需要预先配置好 config.py 的：
    redis_cache['SOCKETIO']
    SOCKETIO_KEY
    SOCKETIO_NAMESPACE
"""

from app import redis_cache

import config
import msgpack


socketio_cache = redis_cache['SOCKETIO']


EVENT = 2
BINARY_EVENT = 5


def get_chn(key, namespace=config.SOCKETIO_NAMESPACE, room=None):
    """获取 socketio subscribe 的 chn（redis subscribe key）"""

    args = [key, namespace, ""]

    if room:
        args = [key, namespace, room, ""]

    return '#'.join(args)


def pack_data(event,
              data,
              uid='py-emitter',
              namespace=config.SOCKETIO_NAMESPACE,
              room=None):
    """组装数据"""
    packet = {
        'data': [
            event,
            data
        ],
        'type': EVENT,
        'nsp': namespace,
    }

    rooms = ''
    if room:
        rooms = []
        rooms.append(room)

    ext = {
        'flags': '',
        'rooms': rooms
    }

    args = [uid, packet, ext]
    # print args

    return msgpack.packb(args)


def emit(room, event, data, namespace=config.SOCKETIO_NAMESPACE):
    """发事件给用户"""

    chn = get_chn(config.SOCKETIO_KEY, namespace=namespace, room=room)

    pdata = pack_data(event, data, namespace=namespace, room=room)

    socketio_cache.publish(chn, pdata)
