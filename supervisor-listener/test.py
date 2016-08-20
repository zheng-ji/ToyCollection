#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import select
import functools
import time
import datetime
import requests
import socket
import logging
import logging.handlers
from collections import defaultdict


send_source = "s-cbded6f8-2a98-4be6-a09a-9631730e"
receiver_group = "g-89e89435-b350-498d-91b4-7ba41f3e"
alarm_frequency = 60
log_file = "xxx"

event_state = {
    "PROCESS_STATE_STARTING": "STARTING",
    "PROCESS_STATE_RUNNING": "RUNNING",
    "PROCESS_STATE_BACKOFF": "BACKOFF",
    "PROCESS_STATE_EXITED": "EXITED",
    "PROCESS_STATE_FATAL": "FATAL",
}

process_states = defaultdict(list)
logger = logging.getLogger('alarm')


def init_logger():
    handler = logging.handlers.RotatingFileHandler(
        log_file, maxBytes=1024 * 1024 * 10, backupCount=5
    )
    fmt = "%(asctime)s - %(name)s - %(levelname)s - %(message)s"
    formatter = logging.Formatter(fmt)
    handler.setFormatter(formatter)
    logger = logging.getLogger('alarm')
    logger.addHandler(handler)
    logger.setLevel(logging.INFO)


def singleton(cls, *args, **kw):
    instances = {}

    def _singleton():
        if cls not in instances:
            instances[cls] = cls(*args, **kw)
        return instances[cls]
    return _singleton


@singleton
class EventListener(object):
    def __init__(self):
        self.is_ready = False

    def get_wait(self, stdin=sys.stdin, stdout=sys.stdout):
        if not self.is_ready:
            self._ready(stdout)
            self.is_ready = True
        line = stdin.readline()
        headers = self._get_dict(line)
        data = stdin.read(int(headers['len']))
        payload = self._get_dict(data)
        self._ok()
        self.is_ready = False
        payload["time"] = datetime.datetime.now().strftime("%H:%M:%S")
        try:
            payload["current_state"] = event_state[headers["eventname"]]
        except:
            payload["current_state"] = "UNKNOWN"
        logger.debug("Receive event. process: {}, event: {}, from_state: {}".format(payload["processname"], headers["eventname"], payload["from_state"]))
        return payload

    def get_no_wait(self, stdin=sys.stdin, stdout=sys.stdout):
        if not self.is_ready:
            self._ready(stdout)
            self.is_ready = True
        r, w, e = select.select([stdin], [], [], 0)
        if len(r) == 0:
            return None
        return self.get_wait(r[0])

    def _ready(self, stdout=sys.stdout):
        stdout.write('READY\n')
        stdout.flush()

    def _get_dict(self, line):
        return dict([x.split(':') for x in line.split()])

    def _ok(self, stdout=sys.stdout):
        self._send('OK', stdout)

    def _fail(self, stdout=sys.stdout):
        self._send('FAIL', stdout)

    def _send(self, data, stdout=sys.stdout):
        result = '%s%d\n%s' % ('RESULT ', len(data), data)
        stdout.write(result)
        stdout.flush()


class Scheduler(object):
    def __init__(self):
        self.jobs = []

    def every(self, interval=1):
        job = Job(interval)
        self.jobs.append(job)
        return job

    def run_pending(self):
        while True:
            runnable_jobs = (job for job in self.jobs if job.should_run)
            for job in sorted(runnable_jobs):
                job.run()
            time.sleep(1)


class Job(object):
    def __init__(self, interval):
        self.interval = interval

    def __lt__(self, other):
        return self.next_run < other.next_run

    def _schedule_next_run(self):
        period = datetime.timedelta(seconds=self.interval)
        self.next_run = datetime.datetime.now() + period

    @property
    def should_run(self):
        return datetime.datetime.now() >= self.next_run

    def do(self, job_func, *args, **kwargs):
        self.job_func = functools.partial(job_func, *args, **kwargs)
        self._schedule_next_run()

    def run(self):
        ret = self.job_func()
        self.last_run = datetime.datetime.now()
        self._schedule_next_run()
        return ret


def collect_process_events():
    listener = EventListener()
    payload = listener.get_no_wait()
    while payload is not None:
        process_states[payload["processname"]].append(payload)
        payload = listener.get_no_wait()


def alert_over(title, message, priority=1):
    url = "https://api.alertover.com/v1/alert"
    payload = {
        "source": send_source,
        'receiver': receiver_group,
        'content': message,
        'title': title,
        'priority': priority,
    }
    if priority != 1:
        payload['sound'] = 'silent'
    requests.post(url, data=payload)


def send_alarm():
    logger.info("Heartbeat every {} seconds".format(alarm_frequency))
    hostname = socket.gethostname()
    curr_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    for process, states in process_states.items():
        if process == "alarm":
            continue
        change_of_state = []
        should_send = False
        final_status = states[-1]["current_state"]
        for state in states:
            origin = state["from_state"]
            curr = state["current_state"]
            t = state["time"]
            if origin in ("EXITED", "FATAL") or curr in ("EXITED", "FATAL"):
                should_send = True
            origin += (8 - len(origin)) * "  "
            curr += (8 - len(curr)) * "  "
            change_of_state.append("    {} →   {}  ({})".format(origin, curr, t))

        if should_send:
            title = "{} {} supervisor监控".format(hostname, process)
            msg = "Process:  %s\nStatus:     %s\nHost:        %s\nTime:       %s\n\nChangeOfStatus:\n%s" % (
                process, final_status, hostname, curr_time, "\n".join(change_of_state))
            priority = 1
            if final_status == "RUNNING":
                priority = 0
            alert_over(title, msg, priority)
            logger.info("Send alarm: " + title)
            logger.debug("Alarm msg:\n" + msg)
    process_states.clear()


def main():
    init_logger()
    sche = Scheduler()
    sche.every(1).do(collect_process_events)
    sche.every(alarm_frequency).do(send_alarm)
    sche.run_pending()


if __name__ == "__main__":
    main()
