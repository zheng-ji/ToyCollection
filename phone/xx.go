// Copyright 2011-2014 YOUMI.NET
// Author: yangxiao@youmi.net
// 防刷模块

package main

import (
	xxh "bitbucket.org/StephaneBunel/xxhash-go"
	"github.com/cihub/seelog"
	"time"
)

type ReqCounter struct {
	beginTime int64 // 记录的第一次请求时间
	count     int32 // 从记录的第一次请求时间开始请求了多少次
}

type AntiBrush struct {
	reqRecords map[uint32]*ReqCounter
}

func NewAntiBrush() *AntiBrush {
	ab := new(AntiBrush)
	ab.reqRecords = make(map[uint32]*ReqCounter)
	return ab
}

// 检查ip+ua是不是被检测为刷接口
func (ab *AntiBrush) checkIfBlocked(ip, ua string, incr bool) (blocked bool) {
	blocked = false

	ipua := ip + ua

	// 检查参数是否正确
	if len(ipua) == 0 {
		blocked = true
		return
	}

	key := xxh.Checksum32([]byte(ipua))
	if val, ok := ab.reqRecords[key]; ok {
		// 多线程不会很准确，但够用了
		if incr {
			val.count = val.count + 1
		}

		duration := time.Now().Unix() - val.beginTime

		// 小于3秒不管
		if duration < 3 {
			return
		}

		// 平均每秒超过N条则block
		if int64(val.count)/duration > int64(appConfig.MaxAccessCountPerSec) {
			blocked = true
			// 每屏蔽几次记录一次log
			if val.count%100 == 0 {
				seelog.Infof("[Blocked] cnt:%d, duration:%d, ip:%s, ua:%s", val.count, duration, ip, ua)
			}
			return
		}
	} else {
		// 不存在则新建一个
		rc := new(ReqCounter)
		rc.beginTime = time.Now().Unix()
		rc.count = 1
		ab.reqRecords[key] = rc
	}

	return
}

// 清理ip+ua信息
func (ab *AntiBrush) cleanRecords() {
	nowTimestamp := time.Now().Unix()
	for key, val := range ab.reqRecords {
		duration := nowTimestamp - val.beginTime
		if duration < 10 {
			continue
		}
		if int64(val.count)/duration < int64(appConfig.MaxAccessCountPerSec) {
			delete(ab.reqRecords, key)
		}
	}
}

// 定期清理ip+ua信息
func (ab *AntiBrush) scheduleCleaning() {
	c := time.Tick(2 * time.Minute)
	for _ = range c {
		ab.cleanRecords()
	}
}
