#! /usr/bin/env python
# -*- coding: utf-8 -*-

"""
微信用到的一些工具方法
"""
import sys
import hashlib
import config

reload(sys)
sys.setdefaultencoding('utf-8')


class SignBackError(Exception):
    """验证微信返回签名异常错误"""
    pass


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
            .strip()\
            .encode('utf8')
        if v:
            # 微信对无值参数是跳过的，只对有值的处理
            k = k.encode('utf8')
            pair_array.append('%s=%s' % (k, v))

    return '&'.join(pair_array)


def md5(str, bin=False):
    """返回MD5特征值"""
    m = hashlib.md5()
    m.update(str)
    if bin:
        return m.digest()
    return m.hexdigest()


def valid_wechat_params_sign(params, secret, sign_key='sign'):
    """
    验证微信参数签名是否正常，目前只支持 MD5 方式的签名！

    @param  params  微信传入的参数，带 sign 字段
    @param  secret  签名用的密钥
    @param  params  签名字段key，一般是 sign

    """
    if not params or not secret or not sign_key:
        return False

    params = params.copy()

    sign = params.pop(sign_key)

    if not sign:
        return False

    valid_sign = get_sign(params, secret)

    if valid_sign and valid_sign == sign:
        return True
    return False


