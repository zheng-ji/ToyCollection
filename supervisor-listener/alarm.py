#!/usr/bin/env python
# -*- coding: utf-8 -*-
#  Author:   zheng-ji@youmi.net

import sys
import httplib
import urllib
import logging
import socket
from datetime import datetime


class EventAlarm():

    def __init__(self):

        self.logger = logging.getLogger()
        formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
        hdlr = logging.FileHandler("/home/ymserver/log/supervisor/alarm.log")
        hdlr.setFormatter(formatter)
        self.logger.addHandler(hdlr)
        self.logger.setLevel(logging.INFO)

    def write_stdout(self, s):
        sys.stdout.write(s)
        sys.stdout.flush()

    def write_stderr(self, s):
        sys.stderr.write(s)
        sys.stderr.flush()

    def get_hostname(self):
        return socket.gethostname()

    def get_current_time(self):
        return datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    def build_msg(self, headers):
        if headers['eventname'] == 'PROCESS_STATE_FATAL':
            status = 'FATL'
        else:
            status = 'OK'

        data = sys.stdin.read(int(headers['len']))
        arr = dict([item.split(':') for item in data.split()])
        msg = "Time: %s\nStatus: %s\nHost: %s\nProcess: %s\nEvent: %s\nFromState: %s" % (
            self.get_current_time(), status, self.get_hostname(),
            arr['processname'], headers['eventname'], arr['from_state'])
        self.logger.info("headers:%s data:%s" % (headers, data))
        return msg

    def main(self):

        while 1:
            self.write_stdout('READY\n')
            line = sys.stdin.readline()
            headers = dict([x.split(':') for x in line.split()])

            priority = -1
            if headers['eventname'] == 'PROCESS_STATE_FATAL':
                priority = 1

            conn = httplib.HTTPSConnection("api.pushover.net:443")
            conn.request("POST", "/1/messages.json",
                         urllib.urlencode({
                             "token": "aEC1fLFf4jspyXis2q2XAPMF5ReWM6",
                             "user": "unpU2QPBjwte39Gd1tScyrciGysXXc",
                             "message": self.build_msg(headers),
                             "priority": priority,
                             "title": 'Supervisor 监控报警',
                         }), {"Content-type": "application/x-www-form-urlencoded"})

            self.write_stdout('RESULT 2\nOK')


if __name__ == '__main__':
    instance = EventAlarm()
    instance.main()
