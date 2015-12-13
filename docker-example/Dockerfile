FROM ubuntu:14.04
MAINTAINER zheng-ji <http://zheng-ji.info>

RUN echo Asia/Shanghai > /etc/timezone && dpkg-reconfigure --frontend noninteractive tzdata

RUN sed -i "s/archive\.ubuntu/mirrors.163/" /etc/apt/sources.list

COPY ping.tar.gz /
RUN tar xvf ping.tar.gz

VOLUME /data

WORKDIR /ping

ENTRYPOINT ["./start.sh"]

