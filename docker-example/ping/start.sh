#!/bin/bash
if [ ! -d /data/log ]; then
    mkdir /data/log
fi

exec ./main >> /data/log/helloweb.access.log
