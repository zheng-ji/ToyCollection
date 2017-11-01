#! /usr/bin/env python
# -*- coding: utf-8 -*-
import time


def datetime_to_timestamp(datetime_obj):
    timestamp = int(time.mktime(datetime_obj.timetuple()) * 1000.0 +
                     datetime_obj.microsecond / 1000.0)
    return timestamp
