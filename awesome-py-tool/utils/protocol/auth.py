# -*- coding: utf-8 -*-

import sys
import hashlib
import config

from utils.timetool.dateutil import get_current_timestamp


class SignBackError(Exception):
    """验证微信返回签名异常错误"""
    pass

class ApiSignError(Exception):
    pass


def get_server_secret(app_id):
    return config.SERVER_API_SECRET_DICT.get(app_id)


def get_sign(params, secret):
    """获取签名"""
    if not isinstance(params, dict):
        raise TypeError('%s is not instance of dict' % params)
    params_str = get_key_value_str(params)
    params_str = '%s&key=%s' % (params_str, secret)

    sign = md5(params_str).upper()
    if config.DEBUG:
        print(params_str, '\nsign=', sign)
    return sign


def get_signed_params(params, secret):
    """更新参数字典，加入 sign 值"""
    sign = get_sign(params, secret)
    params['sign'] = sign
    return params


def get_key_value_str(params):
    """将将键值对转为 key1=value1&key2=value2
    """
    key_az = sorted(params.keys())
    pair_array = []
    for k in key_az:
        v = str(params.get(k, ''))\
            .strip()
            # .encode('utf8')
        if v:
            # 微信对无值参数是跳过的，只对有值的处理
            # k = k.encode('utf8')
            pair_array.append('%s=%s' % (k, v))

    return '&'.join(pair_array)


def md5(str, bin=False):
    """返回MD5特征值"""
    m = hashlib.md5()
    m.update(str.encode("utf-8"))
    if bin:
        return m.digest()
    return m.hexdigest()


def valid_params_sign(params, secret=None, sign_key='sign', check_timestamp=False):
    """
    验证微信参数签名是否正常，目前只支持 MD5 方式的签名！

    @param  params  微信传入的参数，带 app_id, sign 字段
    @param  secret  签名用的密钥
    @param  params  签名字段key，一般是 sign
    @param  check_timestamp  是否需要检查 timestamp，如果取 True，则 params 需要带 timestamp 字段

    """

    app_id = params.get('app_id')

    if not secret:
        secret = get_server_secret(app_id)

    if not params or not secret or not sign_key:
        return False

    if check_timestamp:
        timestamp = params.get('timestamp')
        if not valid_timestamp(timestamp, 180):
            return False

    params = params.copy()
    sign = params.pop(sign_key)

    if not sign:
        return False

    valid_sign = get_sign(params, secret)

    if valid_sign and valid_sign == sign:
        return True
    return False


def valid_timestamp(timestamp, max_diff=180):
    """
        检查 timestamp 是否有效（在最大误差范围内, 默认 ± 180s）
    """

    if not timestamp:
        return False

    if isinstance(timestamp, (str, unicode)):
        if not timestamp.isdigit():
            return False
        timestamp = int(timestamp)

    if not isinstance(timestamp, (int, long)):
        return False

    if abs(timestamp - get_current_timestamp) <= max_diff:
        return True
    return False

