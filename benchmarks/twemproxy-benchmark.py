#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
import redis
import time
import click
import gevent.monkey
gevent.monkey.patch_socket()


@click.group()
def cli1():
    pass

@cli1.command()
@click.option("--limit", "limit", nargs=1, type=click.IntRange(min=1))
@click.option("--concurrent", "concurrent", nargs=1, default=100, type=click.IntRange(min=1))
def ab(limit, concurrent):
    """ready to benchmark"""
    start = time.time()
    click.echo("start at:%s" % start)

    def work(index):
        r = redis.Redis(host='127.0.0.1', port=8877, db=0)
        key = "work_%d" % index
        r.set(key, index)
        count = 0
        while True:
            threads = []
            for i in range(concurrent):
                threads.append(gevent.spawn(work, i))
            gevent.joinall(threads)
            count += concurrent
            click.echo("process %s " % count)
            if limit is not None and count >= limit:
                break
        end = time.time()
        click.echo("spent: %s second" % str(end-start))

cli = click.CommandCollection(sources=[cli1])

if __name__ == "__main__":
    cli()
