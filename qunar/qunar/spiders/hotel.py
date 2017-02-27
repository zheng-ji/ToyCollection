# -*- coding: utf-8 -*-
import time
import scrapy
from selenium import webdriver


class HotelSpider(scrapy.Spider):
    name = "hotel"
    allowed_domains = ["hote.qunar.com"]
    #start_urls = ['http://hotel.qunar.com/city/guangzhou/dt-19593/?tag=guangzhou&fromDate=2017-03-04&toDate=2017-03-09&q=广州喜正家酒店公寓&from=qunarHotel|sug&fromFocusList=0&filterid=78dfac93-c178-4a9d-a15a-86a250fc7792_A&showMap=0&qptype=hotelName|poi&haspoi=1&QHFP=ZSS_A0FC8D55']
    start_urls = ['http://hotel.qunar.com/city/guangzhou/dt-19593/?tag=guangzhou&fromDate=2017-03-04&toDate=2017-03-09&q=%E5%B9%BF%E5%B7%9E%E5%96%9C%E6%AD%A3%E5%AE%B6%E9%85%92%E5%BA%97%E5%85%AC%E5%AF%93&from=qunarHotel%7Csug&fromFocusList=0&filterid=78dfac93-c178-4a9d-a15a-86a250fc7792_A&showMap=0&qptype=hotelName%7Cpoi&haspoi=1&QHFP=ZSS_A0FC8D55&cityurl=guangzhou&HotelSEQ=guangzhou_19593&rnd=1488186398928&sgroup=1&roomNum=1']

    def __init__(self, *args, **kwargs):
        """ 要下在webdriver """
        self.driver = webdriver.Chrome()

    def parse_1(self, response):
        self.driver.get(response.url)
        time.sleep(3)
        source = self.driver.page_source

        selector = scrapy.Selector(text=source)

        rooms = selector.xpath("//div[contains(@class, 'room-type-default')]")
        for room in rooms:
            room_type = room.xpath("//td[contains(@class, 'e2')]/div[@class='promote']/div[@class='js-product']/text()").extract()
            print room_type

            price = room.xpath("//td[contains(@class, 'e6')]/div[@class='js-dprice']/div[contains(@class, 'final-price')]/span[@class='sprice']/text()").extract()
            print price

    def parse(self, response):
        self.driver.get(response.url)
        time.sleep(10)
        source = self.driver.page_source
        print response.url

        selector = scrapy.Selector(text=source)

        rooms = selector.css("div.room-type-default")
        for room in rooms:
            agent = room.css("div.agent-pic") \
                       .css("img::attr(alt)") \
                       .extract_first()
            print agent

            name = room.css("div.promote") \
                       .css("div.js-product::text") \
                       .extract_first()
            print name
            price = room.css('div.js-dprice') \
                        .css('div.origin-price') \
                        .css('span.sprice::text') \
                        .extract_first()
            print price

