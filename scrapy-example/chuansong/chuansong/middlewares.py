import random
import base64
from scrapy.downloadermiddlewares.useragent import UserAgentMiddleware


class RandomProxyMiddleware(object):
    def process_request(self, request, spider):
        #request.meta['proxy'] = "http://proxy.ip.umlife.net"
        #request.meta['proxy'] = "http://proxy.abuyun.com:9020"
        request.meta['proxy'] = "http://172.16.1.30:3128"
        #request.headers['Proxy-Authorization'] = "Basic SDhTM0xYQkNTODlVME1NRDo3RDQ0NUQ5QTIxRTg3RTI5"
         
class RandomUserAgent(UserAgentMiddleware):

    def __init__(self, user_agent):
        self.user_agent = user_agent
        self.ua_list = [
            "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/22.0.1207.1 Safari/537.1",
            "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.6 (KHTML, like Gecko) Chrome/20.0.1092.0 Safari/536.6",
            "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.6 (KHTML, like Gecko) Chrome/20.0.1090.0 Safari/536.6",
            "Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/19.77.34.5 Safari/537.1",
            "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.9 Safari/536.5",
            "Mozilla/5.0 (Windows NT 6.0) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.36 Safari/536.5",
            "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1063.0 Safari/536.3",
            "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1062.0 Safari/536.3",
            "Mozilla/5.0 (Windows NT 6.2) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1061.1 Safari/536.3",
            "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.3 (KHTML, like Gecko) Chrome/19.0.1061.1 Safari/536.3",
            "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/535.24 (KHTML, like Gecko) Chrome/19.0.1055.1 Safari/535.24",
            "Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/535.24 (KHTML, like Gecko) Chrome/19.0.1055.1 Safari/535.24"
        ]

    def process_request(self, request, spider):
        ua = random.choice(self.ua_list)
        if ua:
            request.headers.setdefault('User-Agent', ua.strip())
        request.headers.setdefault('X-Forwarded-For', '%s.%s.%s.%s'.format(
            random.randint(1, 225), random.randint(1, 225),
            random.randint(1, 225), random.randint(1, 225))
        )
