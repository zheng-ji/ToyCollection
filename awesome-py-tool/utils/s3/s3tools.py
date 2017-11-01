#! /usr/bin/env python
# -*- coding: utf-8 -*-

"""
"""
import os
import boto

import config

import logging

logger = logging.getLogger('default')
server_error_logger = logging.getLogger('server_error')


def upload_to_s3(related_path, s3=None):
    """
        上传文件到S3
        related_path 要上传的文件在本地的路径(相对于定义好的本地路径)

    """
    if not related_path:
        return {'c': 4, 'm': "related_path 无效"}

    if not config.SYNC_S3:
        return {'c': 3, 'm': "未开启同步开关 SYNC_S3 = False"}

    bucket_name = config.S3_CONFIG['bucket']
    root_dir = config.S3_CONFIG['root_dir']
    local_dir = config.S3_CONFIG['local_dir']

    if not s3:
        s3 = connect_to_s3()
    if not s3:
        resp_dict = {
            "c": 2,
            "m": "s3连接失败"
        }
        return resp_dict

    bucket = s3.get_bucket(bucket_name)

    file_complete_path = os.path.join(local_dir, related_path)
    s3_path = os.path.join(root_dir, related_path)

    msg = "[upload_to_s3][START] s3 path: %s\nlocal path: %s\n" % (
        s3_path, file_complete_path)
    logger.info(msg)

    upload_success = False
    for count in range(1, 4):
        try:
            k = bucket.new_key(s3_path)
            k.set_contents_from_filename(file_complete_path)
            upload_success = True
            msg = "[upload_to_s3][SUCCESS] s3 path: %s\nlocal path: %s\n" % (
                s3_path, file_complete_path)
            logger.info(msg)
        except Exception as e:
            msg = "[upload_to_s3][FAIL] s3 path: %s\nlocal path: %s\n, error: %s" % (
                s3_path, file_complete_path, e)
            logger.error(msg)
            server_error_logger.error(msg)
        if upload_success:
            break
        #time.sleep(10)
    if not upload_success:
        resp_dict = {
            "c": 1,
            "m": "上传到S3失败"
        }
        return resp_dict

    resp_dict = {
        "c": 0,
        "m": "上传成功"
    }
    return resp_dict


def connect_to_s3():
    """
        连接到s3
    """
    s3 = None
    aws_access_key_id = config.S3_CONFIG['access_key_id']
    aws_secret_access_key = config.S3_CONFIG['secret_access_key']
    region = config.S3_CONFIG['region']

    try:
        s3 = boto.s3.connect_to_region(
            region,
            aws_access_key_id=aws_access_key_id,
            aws_secret_access_key=aws_secret_access_key,
            is_secure=False
        )
    except Exception as e:
        server_error_logger.error("connect fail - %s" % e)
        return None

    return s3
