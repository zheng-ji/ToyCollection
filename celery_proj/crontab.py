#!/usr/bin/env python
# encoding: utf-8
from lib import distribute
from celery.schedules import crontab
from kombu import Queue, Exchange

app = distribute.app
app.conf.update(
    CELERYBEAT_SCHEDULE = {
        'every-minute': {
            'task': 'test_cron',
            'schedule': crontab(minute="*"),
            'args': (16, 13),
        }
    },
    CELERY_INCLUDE=("apps.tasks",),
    BROKER_TRANSPORT_OPTIONS = {'visibility_timeout': 60*60*5},
)

if __name__ == '__main__':
    app.start()
