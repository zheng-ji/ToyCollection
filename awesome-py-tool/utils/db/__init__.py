#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
"""
读写分离的 Session 实现

参考：
https://gist.github.com/trustrachel/6828122
https://gist.github.com/adhorn/b84dc47175259992d406 而来
"""
from flask_sqlalchemy import SQLAlchemy, get_state
from functools import partial

from utils import get_random_string

import sqlalchemy.orm as orm

import config
import logging

db_logger = logging.getLogger('db')


class RoutingSession(orm.Session):

    def __init__(self, db, autocommit=False, autoflush=False, **options):
        self.app = db.get_app()
        self.db = db
        self._model_changes = {}

        # session 唯一 id
        self.sid = get_random_string(length=8)
        orm.Session.__init__(
            self, autocommit=autocommit, autoflush=autoflush,
            bind=db.engine,
            binds=db.get_binds(self.app), **options)

    def get_bind(self, mapper=None, clause=None):

        # 被操作对象的包名
        full_class_name = "%s.%s" % (
            mapper.class_.__module__,
            mapper.class_.__name__
        )

        try:
            state = get_state(self.app)
        except (AssertionError, AttributeError, TypeError) as err:
            db_logger.warning(
                "cant get configuration. default bind. Error:" + err)
            return orm.Session.get_bind(self, mapper, clause)

        """
        If there are no binds configured, connect using the default
        SQLALCHEMY_DATABASE_URI
        """
        if state is None or not self.app.config['SQLALCHEMY_BINDS']:
            if not self.app.debug:
                db_logger.debug("Connecting -> DEFAULT")
            return orm.Session.get_bind(self, mapper, clause)

        elif self._name:
            db_logger.debug("Connecting -> {}".format(self._name))
            return state.db.get_engine(self.app, bind=self._name)

        # 写入请求使用 master
        # 操作被改动过的对象，也使用 master
        elif self._flushing or self._model_changes.get(full_class_name):
            bind_key = getattr(mapper.class_, "__bind_key__", "default")

            self._model_changes[full_class_name] = True
            if self.app.debug:
                action = 'WRITE' if self._flushing else 'READ'
                db_logger.debug(
                    "Connecting -> %s MASTER( %s - %s )" % (
                        action,
                        mapper.class_,
                        bind_key
                    )
                )
            return state.db.get_engine(self.app, bind=bind_key)

        # 其他请求使用 slave
        else:
            bind_key = getattr(mapper.class_, "__bind_key__", "default")
            slave_bind_key = "%s_slave" % bind_key
            if slave_bind_key in self.app.config['SQLALCHEMY_BINDS']:
                bind_key = slave_bind_key

            if self.app.debug:
                db_logger.debug(
                    "Connecting -> READ SLAVE( %s - %s )" % (
                        mapper.class_, bind_key
                    )
                )
            return state.db.get_engine(self.app, bind=bind_key)

    _name = None

    def using_bind(self, name):
        s = RoutingSession(self.db)
        vars(s).update(vars(self))
        s._name = name
        return s


class RouteSQLAlchemy(SQLAlchemy):
    """可读写分离路由的 SQLALCHEMY 对象"""

    def __init__(self, *args, **kwargs):
        SQLAlchemy.__init__(self, *args, **kwargs)
        self.session.using_bind = lambda s: self.session().using_bind(s)

    def create_scoped_session(self, options=None):
        if options is None:
            options = {}
        scopefunc = options.pop('scopefunc', None)
        # print "create_scoped_session - options = ", options
        return orm.scoped_session(
            partial(RoutingSession, self, **options), scopefunc=scopefunc
        )

    def apply_driver_hacks(self, app, info, options):
        """设置一些 create_engine 时的参数"""

        if "isolation_level" not in options and hasattr(config, "ISOLATION_LEVEL"):
            options["isolation_level"] = config.ISOLATION_LEVEL

        if hasattr(config, "ECHO_RAW_SQL"):
            options['echo'] = config.ECHO_RAW_SQL
            # print "update options - ", options

        return super(RouteSQLAlchemy, self).apply_driver_hacks(app, info, options)


def get_page_data(page_qs):
    """获取分页对象的分页信息，以字典形式返回"""
    return {
        'page': page_qs.page,
        'has_next': page_qs.has_next,
        'has_prev': page_qs.has_prev,
        'pages': page_qs.pages,
        'total': page_qs.total
    }


def model_to_dict(model, include_fields=None, exclude_fields=None):
    """将 SQLAlchemy 的 model 转换成字典，可以按需过滤字段"""
    d = {c.name: getattr(model, c.name, None) for c in model.__table__.columns}

    if include_fields:
        tmp_d = {}
        for f in include_fields:
            if f in d:
                tmp_d[f] = d.get(f)
        d = tmp_d
    if exclude_fields:
        for f in exclude_fields:
            if f in d:
                del(d[f])
    return d



