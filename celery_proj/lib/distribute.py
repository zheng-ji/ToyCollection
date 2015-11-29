#!/usr/bin/env python
# encoding: utf-8
from celery import Celery

app = Celery('proj', broker='redis://127.0.0.1:6379/0')
