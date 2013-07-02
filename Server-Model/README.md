Server-Model
============

各种服务器设计模型

一个客户端一个进程模式:one-client-one-process-tcp

一个客户端一个线程模式:one-client-one-thread-tcp

一个客户端一个线程模式(使用mutex):one-client-one-thread-tcp-mutex

tcp并发模型:几个子进程处理连接请求:tcp-concurrent

udp并发模型:几个子进程处理连接请求:udp-concurrent

tcp循环模型：每次只处理一个客户端的请求，处理完后再继续处理下一个,tcp-recycle

udp循环模型：每次只处理一个客户端的请求，,处理完后再继续处理下一个:udp-recycle

io复用循环服务器模型:select

