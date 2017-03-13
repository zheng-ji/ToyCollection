from scrapy_redis.spiders import RedisSpider
import scrapy


class DoubanSpider(RedisSpider):
    name = 'xiecheng'
    start = 'http://m.ctrip.com/webapp/hotel/hoteldetail/4496105.html?days=2&atime=20170307&contrl=2&num=1&biz=1'

    def start_requests(self):
        yield scrapy.Request(
            url=self.start,
            callback=self.parse
        )

    def parse(self, response):
        print response.css('div.js_box_list').extract_first()

