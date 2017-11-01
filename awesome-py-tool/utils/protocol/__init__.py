#!/usr/bin/env python
# -*- coding: UTF-8 -*-
try:
    import ujson as json
except ImportError:
    import json

import time
import binascii
import zlib
import logging
import config

from functools import wraps

from utils.code.base import code_msg
from utils.log.base import error
from flask import request, jsonify

from utils.server.pack import MipayHead, MipayHeadExt, MpEnv
from utils.server import pack
from utils.server import common
from utils.server import ymcoder

default_logger = logging.getLogger('default')


def unpack_api_request(req):
    try:
        if config.TESTING and config.DEBUG:
            # 测试环境
            tmp_cuid = 300012
            session_id = ".CGmctICjMZ2"

            if hasattr(config, "DEV_FAKE_CUID"):
                tmp_cuid = getattr(config, "DEV_FAKE_CUID")
            if hasattr(config, "DEV_FAKE_SESSIONID"):
                session_id = getattr(config, "DEV_FAKE_SESSIONID")

            head = MipayHead(tmp_cuid, 0)
            headExt = MipayHeadExt()
            headExt.Session = session_id
            env = MpEnv(head, headExt, b"")
            env.ReqBody = req.get_data()
            secret = ''
            return env, secret
        else:
            env = pack.unpackRequest(req.get_data())
            if env is False:
                return False, False

            head = env.Head
            cur_time = head.Time
            app_key = head.AppKey[:8]
            hex_appkey = ''.join(
                [hex(ord(x))[2:].zfill(2) for x in app_key])

            if hex_appkey not in config.SECRET_MAP:
                return False, False

            appsecret = config.SECRET_MAP[hex_appkey]
            secret = binascii.unhexlify(appsecret)

            if head.Platform != common.PT_ANDROID \
                    and head.Platform != common.PT_IOS:
                env.Head.Result = common.MP_ERR_PLATFORM
                return False, False

            key = pack.GetKey(secret, cur_time, head.Platform)
            result = ymcoder.mpDecode(env.ReqBody, key)
            if result is False:
                env.Head.Result = common.MP_ERR_DECODE
                return False, False

            body = zlib.decompress(result)
            if body is False:
                env.Head.Result = common.MP_ERR_UNPACK
                return False, False

            env.HeadExt.readFrom(body)
            env.ReqBody = body[env.HeadExt.Len:]
            return env, secret
    except:
        return False, False


def get_params(env):
    try:
        # 获取解密过后的参数
        params = json.loads(env.ReqBody)
        # if config.DEBUG:
        #  df_log.write("params:", str(params))

        # 去掉cid的校验位
        cid = params.get("cid", "")
        params["cid"] = cid[0: 12] if cid else ""
    except:
        params = {}
    return params


def check_params(params, require_list):
    """
        检查参数
    """
    c = 0
    for item in require_list:
        if item not in params:
            c = 10006

            if config.DEBUG:
                msg = '[CHECK_PARAMS] key not found - %s' % item
                default_logger.debug(msg)
            break

    result = code_msg(c)
    return result


def get_cuid(env):
    """
        获取用户id
    """
    return env.Head.Uid


def pack_response(result):
    if config.TESTING and config.DEBUG:
        return jsonify(**result)
    else:
        request.env.ReqBody = json.dumps(result)
        request.env.Head.Time = int(time.time())
        key = pack.GetKey(request._secret, request.env.Head.Time, request.env.Head.Platform)
    return pack.packRequest(request.env, key)


def unpack_request(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        if not request.env:
            error('errors', '%s unpack_request error' % func.__name__)
            return jsonify({'c': 404})

        result = func(*args, **kwargs)
        return pack_response(result)
    return wrapper
