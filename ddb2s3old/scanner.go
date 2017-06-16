package main

import (
	"fmt"
	"github.com/AdRoll/goamz/dynamodb"
	"github.com/fatih/color"
	"os"
	"strings"
	"sync"
	"sync/atomic"
	"time"
)

type Scanner struct {
	ddb       *DDB
	tokenizer *Tokenizer

	segments     int
	deleteMode   bool
	outputFolder string

	finished chan struct{}
	isLimit  bool
	limit    uint64
	counter  uint64
	tick     <-chan time.Time
}

func NewScanner(tokenizer *Tokenizer, ddb *DDB, segments int,
	outputFolder string, deleteMode bool, limit uint64) (scanner *Scanner) {

	if deleteMode {
		color.Red("!!!Be Careful, Run in delete Mode!!!")
	}

	scanner = new(Scanner)

	scanner.segments = segments
	scanner.ddb = ddb
	scanner.tokenizer = tokenizer
	scanner.counter = 0
	scanner.outputFolder = outputFolder
	scanner.deleteMode = deleteMode
	scanner.finished = make(chan struct{})
	scanner.tick = time.Tick(2000 * time.Millisecond)

	if limit > 0 {
		color.Red("limit %d", limit)
		scanner.isLimit = true
		scanner.limit = limit
	}

	go scanner.monitorProcess()

	return scanner
}

func (s *Scanner) ParallelScan() {

	var wg sync.WaitGroup

	for segment := 0; segment < s.segments; segment++ {

		wg.Add(1)
		go func(segment, segments int) {
			s.scanSegment(segment, segments)
			defer wg.Done()
		}(segment, s.segments)
	}

	wg.Wait()

	if !s.isLimit {
		close(s.finished)
	}
}

func (s *Scanner) scanSegment(segment, totalSegment int) {

	var comparison []dynamodb.AttributeComparison
	var attrs []map[string]*dynamodb.Attribute
	var startKey dynamodb.StartKey
	startKey = nil

	fname := fmt.Sprintf(
		"%s/%s_%02d.txt",
		s.outputFolder,
		s.ddb.TableName,
		segment)

	fp, err := os.OpenFile(fname, os.O_CREATE|os.O_RDWR, 0660)
	defer fp.Close()

	if err != nil {
		color.Red("%v", err)
		return
	}

	s.dumpHeader(fp)

loop:
	for {
		select {
		case <-s.finished:
			color.Green("Finishe %d", s.limit)
			break loop
		default:
			attrs, startKey, err = s.ddb.Table.ParallelScanPartial(
				comparison, startKey, segment, totalSegment)
			if err != nil {
				color.Red("Error occur %s\n", err.Error())
			}

			// END loop
			if startKey == nil {
				break
			}
			s.dumpItemsAndDel(fp, attrs)
		}
	}
}

func (s *Scanner) dumpHeader(fp *os.File) {
	__str := strings.Join(s.tokenizer.NeedFields, ",") + "\n"
	fp.WriteString(__str)
}

func (s *Scanner) dumpItemsAndDel(fp *os.File, attrs []map[string]*dynamodb.Attribute) {

	for _, item := range attrs {

		if s.tokenizer.Condition(item) {

			row := []string{}
			for _, field := range s.tokenizer.NeedFields {
				row = append(row, item[field].Value)
			}
			__str := strings.Join(row, ",") + "\n"
			fp.WriteString(__str)

			if s.deleteMode {
				if err := s.ddb.DeleteItem(item); err != nil {
					color.Red("%s", err)
				}
			}

			atomic.AddUint64(&s.counter, 1)

			// 限制条数
			if s.isLimit && atomic.LoadUint64(&s.counter) >= s.limit {
				close(s.finished)
				return
			}
		}
	}
}

func (s *Scanner) monitorProcess() {
	for {
		select {
		case <-s.tick:
			fmt.Printf("Now Processing %d\n", atomic.LoadUint64(&s.counter))
		}
	}
}
