package fasthttp

import (
	"net"
	"runtime"
	"runtime/debug"
	"strings"
	"sync"
	"time"
)

// workerPool serves incoming connections via a pool of workers
// in FIFO order, i.e. the most recently stopped worker will serve the next
// incoming connection.
type workerPool struct {
	// Function for serving server connections.
	// It must leave c unclosed.
	WorkerFunc func(c net.Conn) error

	// Maximum number of workers to create.
	MaxWorkersCount int

	// Logger used by workerPool.
	// 阅读笔记：外部的日志
	Logger Logger

	lock         sync.Mutex
	workersCount int
	mustStop     bool

	ready []*workerChan

	stopCh chan struct{}
}

type workerChan struct {
	t  time.Time
	ch chan net.Conn
}

func (wp *workerPool) Start() {
	// 阅读笔记： 控制开关的 chan struct {}, make (chan struct())
	if wp.stopCh != nil {
		panic("BUG: workerPool already started")
	}
	wp.stopCh = make(chan struct{})
	stopCh := wp.stopCh
	// 阅读笔记：go func + for + selct {case chn, default:sleep}
	go func() {
		for {
			select {
			case <-stopCh:
				return
			default:
				time.Sleep(time.Second)
			}
			wp.clean()
		}
	}()
}

func (wp *workerPool) Stop() {
	if wp.stopCh == nil {
		panic("BUG: workerPool wasn't started")
	}
	close(wp.stopCh)
	wp.stopCh = nil

	// Stop all the workers waiting for incoming connections.
	// Do not wait for busy workers - they will stop after
	// serving the connection and noticing wp.mustStop = true.
	wp.lock.Lock()
	for _, ch := range wp.ready {
		ch.ch <- nil
	}
	wp.ready = nil
	wp.mustStop = true
	wp.lock.Unlock()
}

func (wp *workerPool) clean() {
	// Clean least recently used workers if they didn't serve connections
	// for more than one second.
	wp.lock.Lock()
	chans := wp.ready
	for len(chans) > 1 && time.Since(chans[0].t) > time.Second {
		chans[0].ch <- nil
		copy(chans, chans[1:])
		chans = chans[:len(chans)-1]
		wp.ready = chans
		wp.workersCount--
	}
	wp.lock.Unlock()
}

func (wp *workerPool) Serve(c net.Conn) bool {
	ch := wp.getCh()
	if ch == nil {
		return false
	}
	ch.ch <- c
	return true
}

// 阅读笔记：匿名函数
var workerChanCap = func() int {
	// Use blocking workerChan if GOMAXPROCS=1.
	// This immediately switches Serve to WorkerFunc, which results
	// in higher performance (under go1.5 at least).
	if runtime.GOMAXPROCS(0) == 1 {
		return 0
	}

	// Use non-blocking workerChan if GOMAXPROCS>1,
	// since otherwise the Serve caller (Acceptor) may lag accepting
	// new connections if WorkerFunc is CPU-bound.
	return 1
}()

func (wp *workerPool) getCh() *workerChan {
	var ch *workerChan
	createWorker := false

	wp.lock.Lock()
	chans := wp.ready
	n := len(chans) - 1
	if n < 0 {
		if wp.workersCount < wp.MaxWorkersCount {
			createWorker = true
			wp.workersCount++
		}
	} else {
		ch = chans[n]
		wp.ready = chans[:n]
	}
	wp.lock.Unlock()

	if ch == nil {
		if !createWorker {
			return nil
		}
		vch := workerChanPool.Get()
		if vch == nil {
			vch = &workerChan{
				ch: make(chan net.Conn, workerChanCap),
			}
		}
		// 阅读笔记：强制类型转换
		ch = vch.(*workerChan)
		go func() {
			wp.workerFunc(ch)
			workerChanPool.Put(vch)
		}()
	}
	return ch
}

// 阅读笔记：将处理完毕的channel 放回去
func (wp *workerPool) release(ch *workerChan) bool {
	ch.t = time.Now()
	wp.lock.Lock()
	if wp.mustStop {
		wp.lock.Unlock()
		return false
	}
	wp.ready = append(wp.ready, ch)
	wp.lock.Unlock()
	return true
}

// 阅读笔记：sync.Pool, 这个类设计的目的是用来保存和复用临时对象，以减少内存分配，降低CG压力
var workerChanPool sync.Pool

// 阅读笔记：没有返回值，就意味者将传入参数做修改
func (wp *workerPool) workerFunc(ch *workerChan) {
	var c net.Conn
	var err error

	// 阅读笔记： 典型的错误捕捉。defer func() + recover() 判断是否为空
	defer func() {
		if r := recover(); r != nil {
			wp.Logger.Printf("panic: %s\nStack trace:\n%s", r, debug.Stack())
		}

		if c != nil {
			c.Close()
			wp.release(ch)
		}
	}()

	for c = range ch.ch {
		if c == nil {
			break
		}
		if err = wp.WorkerFunc(c); err != nil && err != errHijacked {
			errStr := err.Error()
			if !strings.Contains(errStr, "broken pipe") && !strings.Contains(errStr, "reset by peer") {
				wp.Logger.Printf("error when serving connection %q<->%q: %s", c.LocalAddr(), c.RemoteAddr(), err)
			}
		}
		if err != errHijacked {
			c.Close()
		}
		c = nil

		if !wp.release(ch) {
			break
		}
	}
}
