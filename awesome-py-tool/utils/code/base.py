#!/usr/bin/env python
# -*- coding: UTF-8 -*-

STATUS_CODE_LIB = {
    0: "success",  # 表示成功
    1: "test",  # 测试状态码
    2: "订单创建成功，等待支付",  # 测试状态码

    # 10000以上才是正式状态码
    # 1-10 接口处理错误
    10001: "请求必备参数校验失败",
    10002: "服务器返回必备参数校验失败",
    10003: "服务器解密失败",
    10004: "IP不合法",
    10005: "获取数据失败",
    10006: "缺少对应参数",
    10007: "数据保存错误",
    10008: "参数取值错误",
    10009: "请求频繁，稍后重试",
    10010: "未登录用户无法操作",
    10011: "数据不存在或已被删除",

    # 100 开始是服务器网络相关异常
    10101: "服务器异常，依赖网络资源无法获取",

    # 200 开始为支付相关错误
}


def get_code_msg(code, data=None):
    """"""
    res = {
        'c': code,
        'm': STATUS_CODE_LIB.get(code, '未知错误码')
    }
    if data:
        res['d'] = data

    return res


#  @DeprecationWarning
def code_msg(code):
    """
        Please use get_code_msg(code) instead"
    """
    return get_code_msg(code)


if __name__ == "__main__":

    for k in sorted(STATUS_CODE_LIB.keys()):
        print(k, "\t", STATUS_CODE_LIB.get(k))
