### goTcpProxy 

A Tcp Proxy Written By Go

Tcp的代理程序


### How To Use

```
Usage of ./pass:
    -src="127.0.0.1:8999": The proxy server's host.
    -dest="172.16.1.250:3306": where proxy server forward requests to.
    -max_avail_conn=25: The max active connection at any given time.
    -max_wait_conn=10000: The max connections that can be waiting to be served.
    -ttl=20: time out of read and write
```

```
./pass -src="127.0.0.1:8999" -dest="172.16.1.250:3306"
```

----

MIT LICENSE

