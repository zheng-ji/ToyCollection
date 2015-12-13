# docker 演习

* 一个web程序
* 日志输出映射到宿主机器上面
* 同时也能连接外部的redis,开放端口

## 过程

```
# 制作image
$ sudo docker build -t test/helloweb .

# 开启容器
$ sudo docker run -p 9998:9999 -v /home/zj/log/helloweb-docker:/data -t -i test/helloweb

```

## output

```
tail -f /home/zj/log/helloweb-docker/log/helloweb.access.log 
同时redis 有输出访问
```
