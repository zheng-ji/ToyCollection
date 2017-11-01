#! /usr/bin/env python
# -*- coding: utf-8 -*-

"""

"""
from datetime import datetime
from calendar import monthrange

import time

DATE_PATTERN = "%Y-%m-%d"
DATETIME_PATTERN = "%Y-%m-%d %H:%M:%S"
MONTH_PATTERN = "%Y-%m"


def simple_format_datetime(dt):
    return dt.strftime(DATETIME_PATTERN)


def simple_format_date(dt):
    return dt.strftime(DATE_PATTERN)


def simple_format_month(dt):
    return dt.strftime(MONTH_PATTERN)


def parse_datetime_str(datetime_str):
    return datetime.strptime(datetime_str, DATETIME_PATTERN)


def parse_date_str(date_str):
    return datetime.strptime(date_str, DATE_PATTERN)


def get_last_day_of_month(year, month):
    """获取指定年月的天数"""
    return monthrange(year, month)[1]


def get_timestamp(dt=None):
    """由 datetime 对象获取对应的时间戳，如: 1460368253

    当 dt 为 None 时获取当前时间的时间戳

    :param dt: datetime 对象
    """
    if not dt:
        return get_current_timestamp
    else:
        if not isinstance(dt, datetime):
            raise TypeError("%s is not a datetime instance." % dt)

    t = time.mktime(dt.timetuple())
    return int(t)


def datetime_str_to_timestamp(datetime_str):
    return get_timestamp(parse_datetime_str(datetime_str))


def date_str_to_timestamp(date_str):
    return get_timestamp(parse_date_str(date_str))

def get_current_timestamp():
    """获取当前时间的时间戳"""
    return int(time.time())
