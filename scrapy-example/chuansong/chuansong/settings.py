# -*- coding: utf-8 -*-

# Scrapy settings for chuansong project
#
# For simplicity, this file contains only the most important settings by
# default. All the other settings are documented here:
#
#     http://doc.scrapy.org/en/latest/topics/settings.html
#
BOT_NAME = 'chuansong'

SPIDER_MODULES = ['chuansong.spiders']
NEWSPIDER_MODULE = 'chuansong.spiders'

DOWNLOAD_DELAY = 0.1

# 除非您 真的 需要，否则请禁止cookies。在进行通用爬取时cookies并不需要， 
# (搜索引擎则忽略cookies)。禁止cookies能减少 CPU 使用率及 Scrapy 爬虫在内存中记录的踪迹，提高性能。
COOKIES_ENABLED = False

# 默认是DEBUG，没必要
LOG_LEVEL = 'INFO'

# 并发是指同时处理的request的数量,我们使用阿布云，一般用的是5个
CONCURRENT_REQUESTS = 5

# 除非您对跟进重定向感兴趣，否则请考虑关闭重定向。 
# 当进行通用爬取时，一般的做法是保存重定向的地址，并在之后的爬取进行解析。 
# 这保证了每批爬取的request数目在一定的数量， 否则重定向循环可能会导致爬虫在某个站点耗费过多资源。
REDIRECT_ENABLED = True

# Enables scheduling storing requests queue in redis.
SCHEDULER = "scrapy_redis.scheduler.Scheduler"

# Ensure all spiders share same duplicates filter through redis.
DUPEFILTER_CLASS = "scrapy_redis.dupefilter.RFPDupeFilter"

# Store scraped item in redis for post-processing.
ITEM_PIPELINES = {
    'scrapy_redis.pipelines.RedisPipeline': 300
}

SCHEDULER_PERSIST = False

REDIS_HOST = 'localhost'
REDIS_PORT = 6379
REDIS_ITEMS_KEY = '%(spider)s:items'



DOWNLOADER_MIDDLEWARES = {
    'chuansong.middlewares.RandomUserAgent': 200,
#    'chuansong.middlewares.RandomProxyMiddleware': 210,
}
