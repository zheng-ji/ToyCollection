#! /usr/bin/env python
# -*- coding: utf-8 -*-

from pyspark import SparkContext
from datetime import datetime, timedelta
import re

regex_cache = {}
aid_list = ['1190488', '1204768', '1190487', '1190489', '1191099', '1204166', '1196580', '1193449', '1204187',
            '1196513', '1192689', '1184572', '1184570', '1203718',
            '1192946', '1184577', '1188916', '1178772', '1178771', '1181142', '1176915', '1194790', '1178892', '43346',
            '1191630']


def parse_regex(field):
    global regex_cache
    pattern = r"%s=([^&]+)" % field
    if pattern not in regex_cache:
        regex_cache[pattern] = re.compile(pattern)
    return regex_cache[pattern]


def line_get(line, field):
    result = line_arr(line, field)
    return result[0] if len(result) > 0 else None


def line_arr(line, field):
    regex = parse_regex(field)
    result = regex.findall(line)
    return result


def date_range(start_date, end_date):
    for i in xrange((end_date - start_date).days):
        yield start_date + timedelta(days=i)


def idfa_app_data(line):
    idfa = line_get(line, "ifa")
    if not idfa:
        return []
    app = line_get(line, "app")
    if not app or (app not in aid_list):
        return []
    ip = line_get(line, "ip")
    if not ip:
        return []
    # return [('ifa=' + idfa + "&" + 'ip=' + ip)]
    return [(idfa, ip)]


def reduce_idfa_app_data(a, b):
    if len(a[1]) == 0 and len(b[1]) != 0:
        return b
    return a


def dump(line):
    return "ip=" + line[1] + "&ifa=" + line[0]


if __name__ == "__main__":
    DEBUG = False
    sc = SparkContext(appName="bigmedia_data")
    if not DEBUG:
        textFile = None
        textFile = sc.textFile("/home/zj/tool/py/learnspark/tmp.log")

        tmp1 = textFile.flatMap(idfa_app_data)
        print "12111", tmp1.collect()

        print "=============="

        tmp2 = textFile.map(idfa_app_data)
        print "12112", tmp2.collect()

        idfa_data_rdd = textFile.flatMap(idfa_app_data).reduceByKey(reduce_idfa_app_data)

        save_rdd = idfa_data_rdd.map(dump)
        #print '1234', save_rdd.collect()

        save_rdd.persist()
        # save
        s3_path_match = "/home/zj/tool/py/learnspark/tiantao"
        save_rdd.saveAsTextFile(s3_path_match)
