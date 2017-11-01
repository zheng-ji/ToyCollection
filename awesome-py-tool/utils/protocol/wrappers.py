#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2016 Hector Qiu <hectorqiuiscool@gmail.com>

"""

"""
import ujson as json
import config

from collections import Iterable
from datetime import datetime, date
from flask import request, jsonify
from flask.globals import current_app
from flask.json import JSONEncoder
from flask.wrappers import Request
from utils.code.base import STATUS_CODE_LIB
from utils.timetool import dateutil

from utils.protocol.yiisessionutil import RedisSession


class YmApiRequest(Request):
    """有米 request 对象
    """

    @property
    def ip(self):
        return request.environ.get(
            'HTTP_X_REAL_IP', request.remote_addr)

    @property
    def session_id(self):
        return self.headers.get('session-id')

    @property
    def user_info(self):
        redis_cache = current_app.extensions['redis_cache']
        session = redis_cache['SESSION']
        session_key = 'user.session?session_id=%s' % self.session_id
        user_info = session.get(session_key)
        if user_info:
            return json.loads(user_info)
        else:
            return {}

    @property
    def user_id(self):
        default_user_id = 0
        if config.DEBUG and config.TESTING:
            default_user_id = config.FAKE_USER_ID

        return self.user_info.get('user_id', default_user_id)

    @property
    def openid(self):
        return self.user_info.get('openid', None)

    @property
    def unionid(self):
        return self.user_info.get('unionid', None)

    @property
    def page_from_json(self):
        """在json data 内获取 page 参数"""
        page = 1
        try:
            if self.get_json():
                page = int(self.get_json().get('page', '1'))
        except ValueError:
            pass
        return max(1, page)  # 最小应该是 1

    @property
    def page_from_args(self):
        """在json data 内获取 page 参数"""
        page = 1
        try:
            page = int(self.args.get('page', '1'))
        except ValueError:
            pass
        return max(1, page)  # 最小应该是 1

    @property
    def page_size_from_json(self):
        """在json data 内获取 page_size 参数"""
        page_size = 20
        if self.get_json():
            page_size = int(self.get_json().get('page_size', '20'))
        return min(30, page_size)  # 限制最大不能超过 30

    @property
    def page_size_from_args(self):
        """在json data 内获取 page_size 参数"""
        page_size = int(self.args.get('page_size', '20'))
        return min(30, page_size)  # 限制最大不能超过 30


class NBMSApiRequest(YmApiRequest):
    """NBMS 专用的 Request 对象"""

    _redis_session = None

    @property
    def redis_session(self):
        if not self._redis_session:
            redis_cache = current_app.extensions['redis_cache']
            redis = redis_cache['SESSION']
            self._redis_session = RedisSession(redis)
        return self._redis_session

    @property
    def session_id(self):
        """读 php 项目的 session id"""

        sess_id = self.cookies.get('PHPSESSID')
        if not sess_id:
            sess_id = self.headers.get('session-id')
        return sess_id

    @property
    def user_info(self):
        info = self.redis_session.read_session(self.session_id)

        if not info:
            info = {}
        return info

    @property
    def user_id(self):
        default_user_id = 0
        if config.DEBUG and config.TESTING:
            default_user_id = config.FAKE_USER_ID

        return self.user_info.get('__id', default_user_id)

    @property
    def openid(self):
        return self.user_info.get('openid', None)

    @property
    def unionid(self):
        return self.user_info.get('unionid', None)



def render_api_response(code_or_dict):
    """
        通过接口协议获取 json 返回结果

        @input:
            code_or_dict:
                当类型为 int 时，获取应用预定义的返回 reponse
                    预定义错误码见 utils.code.base
                当类型为 dict 时，返回加密 reponse

        @return:
            json 格式 reponse 对象
    """
    resp_dict = {}
    if isinstance(code_or_dict, int):
        resp_dict = {
            "c": code_or_dict,
            "m": STATUS_CODE_LIB.get(code_or_dict),
        }
    elif isinstance(code_or_dict, dict):
        resp_dict = code_or_dict
    else:
        """类型错误"""
        raise TypeError(
            "code_or_dict should be int or dict object，%s found."
            % type(code_or_dict))

    return jsonify(resp_dict)


class ReverseProxied(object):
    '''Wrap the application in this middleware and configure the
    front-end server to add these headers, to let you quietly bind
    this to a URL other than / and to an HTTP scheme that is
    different than what is used locally.

    In nginx:
    location /myprefix {
        proxy_pass http://192.168.0.1:5001;
        proxy_set_header Host $host;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header X-Scheme $scheme;
        proxy_set_header X-Script-Name /myprefix;
        }

    :param app: the WSGI application
    '''
    def __init__(self, app):
        self.app = app

    def __call__(self, environ, start_response):
        script_name = environ.get('HTTP_X_SCRIPT_NAME', '')
        if script_name:
            environ['SCRIPT_NAME'] = script_name
            path_info = environ['PATH_INFO']
            if path_info.startswith(script_name):
                environ['PATH_INFO'] = path_info[len(script_name):]

        scheme = environ.get('HTTP_X_SCHEME', '')
        if scheme:
            environ['wsgi.url_scheme'] = scheme
        return self.app(environ, start_response)


class YmJsonEncoder(JSONEncoder):
    """加入对 datetime date 的转化，同时支持 Iterable 子类的序列化"""

    def default(self, obj):
        if isinstance(obj, date):
            return dateutil.simple_format_date(obj)
        elif isinstance(obj, datetime):
            return dateutil.simple_format_datetime(obj)
        elif isinstance(obj, Iterable) and not isinstance(obj, (dict, list, set)):
            # 这个实现可能有问题
            return JSONEncoder.default(self, list(obj))

        return JSONEncoder.default(self, obj)
