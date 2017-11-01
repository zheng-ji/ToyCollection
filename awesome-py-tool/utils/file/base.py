#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2013 Youmi
"""
"""
import config
import logging
import os
import shutil
import requests

from hashlib import md5
from urllib.request import urlopen
from urllib.error import URLError, HTTPError
from utils import get_random_string
from utils.s3 import s3tools
from requests.exceptions import RequestException

from datetime import datetime

logger = logging.getLogger('network')

TWO_MIN = 120

FIVE_MIN = 300


def md5_file(file):
    """获取文件md5"""
    m = md5()
    a_file = open(file, 'rb')
    m.update(a_file.read())
    a_file.close()
    return m.hexdigest()


def wget_file(url, dest_path, timeout=None):
    """获取某个url指定的文件"""
    logger.info("[wget_file] start url - %s path - %s" % (url, dest_path))
    try:
        f = None
        if timeout:
            f = urlopen(url, timeout=timeout)
        else:
            f = urlopen(url)
        dest_dir = os.path.dirname(dest_path)
        if not os.path.isdir(dest_dir):
            os.makedirs(dest_dir)
        with open(dest_path, "w") as local_file:
            local_file.write(f.read())
            local_file.close()
            return 0, None
        logger.info("[wget_file] done url - %s" % url)
    except HTTPError as e:
        print(e.code)
        return 1, e
    except URLError as e:
        print(e.reason)
        return 2, e


def download_file(url, dest_path, timeout=5, max_retry=3):
    """下载文件到指定路径

    @param url
    @param dest_path
    @param timeout 响应超时，单位：秒
    """
    do_retry = 1

    logger.info("[download_file] start url - %s path - %s" % (url, dest_path))
    while do_retry <= max_retry:
        try:
            do_retry += 1
            res = requests.get(url, stream=True, timeout=timeout)
            if res.status_code == 200:
                with open(dest_path, 'wb') as out_file:
                    shutil.copyfileobj(res.raw, out_file)
                    logger.info("[download_file][SUCCESS] url - %s path - %s" % (url, dest_path))
                return 0, None
            else:
                if do_retry > max_retry:
                    logger.info("[download_file][FAIL] reach max retry time - %s url - %s path - %s" % (
                        max_retry, url, dest_path))
                    return 1, None
        except RequestException as e:
            if do_retry <= max_retry:
                logger.info("[download_file][FAIL]  url - %s path - %s retry - %s err - %s" % (
                    url,
                    dest_path,
                    do_retry,
                    e
                ))
                continue
            else:
                return 2, e


def upload_file_by_url(url, related_dir, default_ext=''):
    ext = get_ext(url)
    if not ext:
        ext = default_ext
    dest_filename = gen_filename(ext)
    tmp_file_path = os.path.join(config.UPLOAD_DIR, related_dir, dest_filename)
    mkdir_if_not_exists(tmp_file_path)
    code, error = wget_file(url, tmp_file_path, timeout=20000)

    if code != 0:
        return False

    # 上传 s3
    related_path = os.path.join(related_dir, dest_filename)
    s3tools.upload_to_s3(related_path)
    return related_path


def get_ext(filename):
    """获取文件名后缀（小写），如: .jpg"""
    return os.path.splitext(filename)[1].lower()


def get_filename(file_path, ext=True):
    """获取文件路径

    """
    filename = os.path.basename(file_path)
    if not ext:
        filename = os.path.splitext(filename)[0]
    return filename


suffixes = ['B', 'KB', 'MB', 'GB', 'TB', 'PB']


def format_filesize(nbytes):
    if nbytes == 0:
        return '0 B'

    i = 0
    while nbytes >= 1024 and i < len(suffixes) - 1:
        nbytes /= 1024.
        i += 1
    f = ('%.2f' % nbytes).rstrip('0').rstrip('.')
    return '%s %s' % (f, suffixes[i])


def get_today_dir():
    """获取当天的文件存储目录，如：2016/11/01"""
    now = datetime.now()

    return "%s/%02d/%02d" % (now.year, now.month, now.day)


def gen_filename(file_ext):

    name = "%s%s" % (
        get_random_string(30),
        file_ext
    )

    filename = os.path.join(
        get_today_dir(),
        name
    )
    return filename


def mkdir_if_not_exists(dest_file_path):
    """判断目标路径的目录是否存在，不存在则创建目录
    """
    dest_dir = os.path.dirname(dest_file_path)
    if not os.path.isdir(dest_dir):
        os.makedirs(dest_dir)
