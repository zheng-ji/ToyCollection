from kazoo.client import KazooClient
import time

zk = KazooClient(hosts="172.16.1.250:2181,172.16.1.250:2182,172.16.1.250:2183")
zk.start()

@zk.DataWatch("/xj")
def changed(data, stat, event):
    print "--------------DataWatch---------------"
    print "data:", data
    print "stat:", stat
    print "event:", event


zk.create("/xj", "value1")
time.sleep(2)
zk.set("/xj", "value2")
time.sleep(2)
zk.delete("/xj")
time.sleep(2)
