#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#

import logging


def write_log(info, name='default'):
    logger = logging.getLogger(name)
    logger.error(info)
    return True


def debug(logger_name, msg):
    """输出日志到指定 logger
    """
    log_msg(logger_name, msg, level='debug')


def info(logger_name, msg):
    """输出日志到指定 logger
    """
    log_msg(logger_name, msg, level='info')


def warning(logger_name, msg):
    """输出日志到指定 logger
    """
    log_msg(logger_name, msg, level='warning')


def error(logger_name, msg):
    """输出日志到指定 logger
    """
    log_msg(logger_name, msg, level='error')


def critical(logger_name, msg):
    """输出日志到指定 logger
    """
    log_msg(logger_name, msg, level='critical')


def log_msg(logger_name, msg, level='info'):
    """ 输出信息到日志（可以多个）
    """
    names = None
    if isinstance(logger_name, list):
        names = logger_name
    else:
        names = [logger_name, ]

    for name in names:
        logger = logging.getLogger(name)

        if logger:
            level = level.lower()

            if level == 'debug':
                logger.debug(msg)
            elif level == 'info':
                logger.info(msg)
            elif level == 'warn' or level == 'warning':
                logger.warning(msg)
            elif level == 'error':
                logger.error(msg)
            elif level == 'critical':
                logger.critical(msg)
