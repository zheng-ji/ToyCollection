from kazoo.client import KazooClient
from kazoo.client import KazooState

import logging
logging.basicConfig()

zk = KazooClient(hosts='172.16.1.250:2181, 172.16.1.250:2182, 172.16.1.250:2183', read_only=True)

def my_listener(state):
    if state == KazooState.LOST:
        # Register somewhere that the session was lost
        print "lost stat"
    elif state == KazooState.SUSPENDED:
        # Handle being disconnected from Zookeeper
        print "SUSPENDED stat"
    else:
        # Handle being connected/reconnected to Zookeeper
        print "else stat"

zk.add_listener(my_listener)
zk.start()
zk.stop()
