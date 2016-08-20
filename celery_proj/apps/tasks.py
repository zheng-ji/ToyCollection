#!/usr/bin/env python
# encoding: utf-8
from lib.distribute import app
import requests
import logging

logger = logging.getLogger("watcher")
logger.setLevel(logging.INFO)
log_format = "[%(asctime)s] [%(levelname)s] %(message)s"
log_time_format = "%Y-%m-%d %H:%M:%S"
fh = logging.FileHandler("/tmp/celery-worker.log")
formatter = logging.Formatter(log_format, log_time_format)
fh.setFormatter(formatter)
logger.addHandler(fh)
logger.propagate = False


@app.task
def add(x, y):
    r = requests.get("http://www.apple.com/cn/support/itunes/")
    logger.info(r.text[2:10])
    return x + y


@app.task(name='test_cron')
def mul(x, y):
    logger.info("hello world")
    return x * y


@app.task
def xsum(numbers):
    return sum(numbers)
