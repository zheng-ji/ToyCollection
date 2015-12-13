package main

import (
	"fmt"
	"github.com/garyburd/redigo/redis"
	"github.com/hoisie/web"
	"time"
)

var redisPool *redis.Pool

func NewRedisPool(host string, db string) *redis.Pool {
	redisPool = &redis.Pool{
		MaxIdle:     3,
		IdleTimeout: 1 * time.Hour,
		Dial: func() (redis.Conn, error) {
			c, err := redis.Dial("tcp", host)
			if err != nil {
				fmt.Println("[Redis] Dial erro: %v", err)
				return nil, err
			}
			if _, err := c.Do("SELECT", db); err != nil {
				c.Close()
				fmt.Println("[Redis] Select DB error: %v", err)
				return nil, err
			}
			return c, err
		},
		TestOnBorrow: func(c redis.Conn, t time.Time) error {
			_, err := c.Do("PING")
			return err
		},
	}

	return redisPool
}

func hello(ctx *web.Context, val string) string {
	fmt.Println("%s", ctx.Params)
	redisPool = NewRedisPool("172.16.5.39:6379", "0")
	conn := redisPool.Get()
	key := "hello"
	v, _ := conn.Do("GET", key)
	return "hello " + string(v.([]byte))
}
func ping(ctx *web.Context) string {
	ret := "ok"
	return ret
}

func main() {
	web.Get("/ping", ping)
	web.Get("/(.*)", hello)
	web.Run("0.0.0.0:9999")
}
