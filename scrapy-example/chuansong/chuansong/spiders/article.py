# -*- coding: utf-8 -*-


import scrapy
import urlparse
from chuansong.items import ArticleItem


class ChuanSongSpider(scrapy.Spider):
    name = "chuansong"
    allowed_domains = []
    start_urls = [
        "http://chuansong.me",
    ]

    def __init__(self, *args, **kwargs):
        super(ChuanSongSpider, self).__init__(*args, **kwargs)
        self.index = 1
        self.start = 25
        self.total = 50

    def parse(self, response):

        # parse articles
        url = response.xpath("//a[@class='question_link']/@href").extract()
        article_urls = list(response.xpath("//a[@class='question_link']/@href").extract())
        print article_urls

        for _url in article_urls:
            url = urlparse.urljoin("http://chuansong.me", _url)
            print url
            req = self.make_requests_from_url(url).replace(callback=self.parse_article)
            yield req

        # parse next page
        if self.index * self.start <= self.total:
            next_page_url = "http://chuansong.me/?start=%d" % (self.start * self.index)
            print next_page_url
            req = self.make_requests_from_url(next_page_url)
            yield req
            self.index = self.index + 1

    def parse_article(self, response):
        url = response.url
        title = response.xpath("//h2[@class='rich_media_title']/text()").extract()
        yield ArticleItem(url=url, title=title)
