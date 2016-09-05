#!/usr/bin/env python
# encoding: utf-8

import re

def parseLog(self, _str):
    ip = r"?P<ip>[\d.]*"
    timeLocal=r"?P<timeLocal>.*"
    method = r"?P<method>\S+"
    request = r"?P<request>\S+"
    status = r"?P<status>\d+"
    bytesSent = r"?P<bytesSent>\d+"
    requestBody = r"?P<requstBody>[^\"]*"
    refer = r"?P<refer>[^\"]*"
    userAgent=r"?P<userAgent>.*"
    requestTime = r"?P<requestTime>[\d.]*"
    cookie=r"?P<cookie>.*"

    p = re.compile(r"(%s)\ -\ -\ \[(%s)\]\ \"(%s)?[\s]?(%s)?.*?\"\ (%s)\ (%s)\ (%s)\ \"(%s)\"\ \"(%s).*?\"\ (%s)\ \"(%s).*?\"" % (
        ip, timeLocal, method, request, status, bytesSent, requestBody, refer, userAgent, requestTime, cookie), re.VERBOSE)

    m = re.findall(p, _str)[0]
    _map = {}
    _map['ip'] = m[0]
    tm = time.strptime(m[1].split(' ')[0], '%d/%b/%Y:%H:%M:%S')
    _map['@timestamp'] = "%04d-%02d-%02dT%02d:%02d:%02dZ" % (tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec)
    _map['method'] = m[2]
    _map['request'] = m[3]
    _map['status'] = m[4]
    _map['requestBody'] = unquote(m[6])
    _map['refer'] = m[7]
    _map['userAgent'] = m[8]
    _map['requestTime'] = m[9]
    _map['cookie'] = m[10]
    return _map

if __name__ == "__main__":
    _str = """
    36.110.105.133 - - [05/Sep/2016:15:33:00 +0800] "POST /Tool/check_mac HTTP/1.1" 200 2579 _method=POST&data%5BWall%5D%5Bmac%5D=&data%5BWall%5D%5Bifa%5D=8E259D11-CC09-EC78-68BC-A84A69ED2BF97DDAF320-A573-4A46-BB7A-8064B9BC4AB5&data%5BWall%5D%5Bdate%5D=2016-09-05 "https://abcd.cn/Tool/check_mac" "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.106 Safari/537.36" 1.599 "8kd2bv1edetvl7ntga2vsr7ao5"
    """
    parseLog(_str)
