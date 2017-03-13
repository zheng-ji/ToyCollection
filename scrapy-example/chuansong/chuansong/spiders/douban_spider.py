from scrapy_redis.spiders import RedisSpider
import scrapy


class DoubanSpider(RedisSpider):
    name = 'douban'
    start = 'https://movie.douban.com/subject/1292052'

    headers = {
        'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10.11; rv:51.0) Gecko/20100101 Firefox/51.0',
        'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
        'Accept-Language': 'zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3',
        'Accept-Encoding': 'gzip, deflate, br',
        'Connection': 'keep-alive',
        'Host': 'movie.douban.com',
        'Upgrade-Insecure-Requests': '1',
        'Cache-Control': 'max-age=0'
    }
    id = 1
    def start_requests(self):
        yield scrapy.Request(
            url=self.start,
            headers=self.headers,
            callback=self.parse
        )

    def parse(self, response):
        data = {
            'title': response.css('title::text').extract_first(),
            'description': response.css('span[property="v:summary"]::text').extract_first()
        }
        print data['title']
        yield {"data": "%s" % str(self.id)}
	self.id = self.id + 1
        others = response.css('div.recommendations-bd a::attr(href)').extract()
        for url in others:
            yield scrapy.Request(
                url=url,
                headers=self.headers,
                callback=self.parse
            )
