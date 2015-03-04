import subprocess
import re
import gevent


class Ping():

    def __init__(self):
        pass

    def run(self):
        gevent.joinall([
            gevent.spawn(self.worker, "zheng-ji.info"),
            gevent.spawn(self.worker, "everet.org"),
            gevent.spawn(self.worker, "baidu.com"),
            gevent.spawn(self.worker, "coolshell.cn"),
            gevent.spawn(self.worker, "weibo.com"),
        ])

    def worker(self, server):
        try:
            result = subprocess.check_output(['ping', '-c 5', server])
            self.check_result(result, server)
        except subprocess.CalledProcessError, er:
            print "server %s error: %s" % (server, er)

    def check_result(self, result_text, server):
        lost_re = re.compile(r'(\d{1,3}\.\d{1,3})% packet loss')
        lost_result = lost_re.search(result_text)
        if lost_result:
            if lost_result.group(1) == '0.0':
                avg_re = re.compile(r'\/(\d{1,3}\.\d{3})')
                avg_result = avg_re.search(result_text)
                if avg_result:
                    delay = float(avg_result.group(1))
                    print "found server %s 0 packet loss, delay %s" % (server, delay)
            else:
                avg_re = re.compile(r'\/(\d{1,3}\.\d{3})')
                avg_result = avg_re.search(result_text)
                if avg_result:
                    delay = float(avg_result.group(1))
                    print "found server %s, %s packet loss, delay %s" % (server, lost_result.group(), delay)
                return
        else:
            print "not loss patter, %s" % result_text

instance = Ping()
instance.run()
