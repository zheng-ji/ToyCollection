#!/usr/bin/env python
# encoding: utf-8
from lib import distribute
from kombu import Queue, Exchange
from celery.schedules import crontab

app = distribute.app
app.conf.update(
    CELERY_INCLUDE=("apps.tasks",),
    CELERY_QUEUES=(
        Queue('test', Exchange('test_exchange'), routing_key='test_queue'),
    ),
    CELERYBEAT_SCHEDULE = {
        'every-minute': {
            'task': 'test_cron',
            'schedule': crontab(minute="*"),
            'args': (16, 13),
        }
    },
    BROKER_TRANSPORT_OPTIONS = {'visibility_timeout': 60*60*5},
)

if __name__ == '__main__':
    app.start()
