#!/usr/bin/env python
# encoding: utf-8
from lib import distribute
from kombu import Queue, Exchange

app = distribute.app
app.conf.update(
    CELERY_QUEUES=(
        Queue('test', Exchange('test_exchange'), routing_key='test_queue'),
    ),
    CELERY_INCLUDE=("apps.tasks",),
    BROKER_TRANSPORT_OPTIONS = {'visibility_timeout': 60*60*5},
)

if __name__ == '__main__':
    app.start()
