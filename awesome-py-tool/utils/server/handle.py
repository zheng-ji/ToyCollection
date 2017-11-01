from . import pack
from . import common
import binascii
from . import ymcoder
import zlib
import time
import json


# function: handleRequest
# args: request body
# return response Body


def handleRequest(req):
    env = pack.unpackRequest(req)
    if env is False:
        return False

    head = env.Head
    cur_time = head.Time
    appkey = head.AppKey[:8]
    #
    # 通过appkey可以在游戏表里面获取appsecret
    #
    appsecret = appkey
    secret = binascii.unhexlify(appsecret)

    if head.Platform != common.PT_ANDROID and head.Platform != common.PT_IOS:
        env.Head.Result = common.MP_ERR_PLATFORM
        return pack.packRequest(env, "")

    key = pack.GetKey(secret, cur_time, head.Platform)
    result = ymcoder.mpDecode(env.ReqBody, key)
    if result is False:
        env.Head.Result = common.MP_ERR_DECODE
        return pack.packRequest(env, "")

    body = zlib.decompress(result)
    if body is False:
        env.Head.Result = common.MP_ERR_UNPACK
        return pack.packRequest(env, "")

    env.HeadExt.readFrom(body)
    env.ReqBody = body[env.HeadExt.Len:]

    retenv = handleProcess(env)
    retenv.Head.Time = int(time.Time())
    key = pack.GetKey(secret, retenv.Head.Time, head.Platform)
    return pack.packRequest(env, key)


def handleProcess(env):

    cmd = env.Head.Cmd
    body = json.loads(env.ReqBody)
    #
    # 通过cmd获取相应的操作
    #
    print(cmd, body)
    env.ReqBody = json.dumps(body)
    return env
