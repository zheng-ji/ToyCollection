#!/usr/bin/env python
# encoding: utf-8
from lib import distribute

app = distribute.app
app.conf.update(
    CELERY_INCLUDE=("apps.tasks",)
)

if __name__ == '__main__':
    app.start()
