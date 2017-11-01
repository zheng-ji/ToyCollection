import struct
import zlib
from . import ymcoder
from . import common


def GetAppKey(appkey):
    if len(appkey) == 16:
        return str(appkey)
    else:
        return appkey.encode('hex')


def GetKey(secret, times, platform):
    PF_KEY = [
        b"\xfc\x1f\x9f\x2f\x0a\xcf\x32\xef\x9e\x8c\xda\x3a\x74\x2e\x78\x65",
        b"\xea\x2f\x5a\x2f\x1f\x3e\xdf\x29\x2a\xce\xfa\x9b\x2e\xba\xbe\x0f",
    ]
    pfkey = ""
    if platform == 0x3:
        pfkey = PF_KEY[0]
    elif platform == 0x5:
        pfkey = PF_KEY[1]
    else:
        return False
    s = struct.pack(">8sI16s", secret[:8], times, pfkey)
    return s


def packRequest(env, key):
    body = b""
    if env.Head.Result == 0:
        tmp = env.HeadExt.writeTo()
        format1 = ">%ds" % len(env.ReqBody)
        tmp = tmp + struct.pack(format1, env.ReqBody)
        tmp2 = zlib.compress(tmp)
        body = ymcoder.mpEncode(tmp2, key)

    env.Head.Len = common.MPHEAD_LEN + 2 + len(body)
    tmp3 = struct.pack(">B", common.MP_STX)
    tmp3 += env.Head.writeTo()

    if len(body) > 0:
        formatbody = ">%ds" % len(body)
        tmp3 += struct.pack(formatbody, body)

    tmp3 += struct.pack(">B", common.MP_ETX)
    return tmp3


def unpackRequest(req):

    head = MipayHead(0, 0)
    headExt = MipayHeadExt()
    env = MpEnv(head, headExt, b"")

    req_len = len(req)
    if req_len < common.MPHEAD_LEN + 2:
        return False

    if int(req[0].encode('hex')) != common.MP_STX or int(req[req_len-1].encode('hex')) != common.MP_ETX:
        return False

    head = env.Head
    head.readFrom(req[1:common.MPHEAD_LEN+1])

    if head.Len != req_len:
        return False
    env.ReqBody = req[common.MPHEAD_LEN+1: req_len-1]
    return env


class MipayHead:
    def __init__(self, uid, platform):
        self.Len = 0
        self.Ver = 0
        self.Cmd = 0
        self.Platform = platform
        self.Option = 0
        self.Uid = uid
        self.Time = 0
        self.Result = 0
        self.Sdkver = 0
        self.AppKey = b""
        self.Reserve = b""

    def writeTo(self):
        s = struct.pack(">HHHBBIIHH8s16s",
                        self.Len, self.Ver,
                        self.Cmd, self.Platform,
                        self.Option, self.Uid,
                        self.Time, self.Result,
                        self.Sdkver, self.AppKey[:8], self.Reserve)
        return s

    def readFrom(self, byte):
        self.Len, self.Ver, self.Cmd, self.Platform = struct.unpack(">HHHB", byte[:7])
        self.Option, self.Uid, self.Time, self.Result = struct.unpack(">BIIH", byte[7:18])
        self.Sdkver, self.AppKey, self.Reserve = struct.unpack(">H8s16s", byte[18:])


class MipayHeadExt:
    def __init__(self):
        self.Len = 4
        self.Type = 0
        self.Slen = 0
        self.Session = ""

    def writeTo(self):
        s = ""
        if self.Slen > 0:
            formatForExt = ">HBB%ds" % self.Slen
            s += struct.pack(formatForExt,
                             self.Len, self.Type, self.Slen,
                             self.Session[:self.Slen])
        else:
            formatForExt = ">HBB"
            s += struct.pack(formatForExt,
                             self.Len, self.Type, self.Slen)
        return s

    def readFrom(self, byte):
        formatForExt = ">HBB"
        self.Len, self.Type, self.Slen = struct.unpack(formatForExt, byte[:4])
        if self.Len < 4:
            return False
        if self.Len > 4 and self.Slen > 0 and self.Slen <= self.Len - 4:
            formatForExt = ">%ds" % self.Slen
            self.Session = (struct.unpack(formatForExt, byte[4:4+self.Slen]))[0]


class MpEnv:
    def __init__(self, head, ext, req):
        self.Head = head
        self.HeadExt = ext
        self.ReqBody = req
