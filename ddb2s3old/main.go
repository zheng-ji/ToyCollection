package main

import (
	"flag"
	"fmt"
	"github.com/fatih/color"
	"os"
)

var (
	region    = flag.String("region", "", "eg. cn-north-1")
	accessKey = flag.String("accesskey", "", "aws access key.")
	secretKey = flag.String("secretkey", "", "aws secret key.")

	delete = flag.Bool("delete", false, "Dry run without doing")
	limit  = flag.Uint64("limit", 0, "num of entry to limit")

	segment      = flag.Int("segment", 1, "Segment num")
	whereStr     = flag.String("where", "SQL String", "select a,b from xx where yy = 1")
	outputFolder = flag.String("output", "", "Your Output Folder.")
)

func prepare() {

	flag.Parse()

	if "" == *region {
		color.Red("!!Region required!!")
		os.Exit(1)
	}

	if "" == *accessKey {
		color.Red("!!access Key required!!")
		os.Exit(1)
	}

	if "" == *secretKey {
		color.Red("!!secret Key required!!")
		os.Exit(1)
	}

	if "" == *whereStr {
		color.Red("!!SQL required!!")
		os.Exit(1)
	}

	if "" == *outputFolder {
		color.Red("!!output Folder required!!")
		os.Exit(1)

	} else {

		if _, err := os.Stat(*outputFolder); os.IsNotExist(err) {
			color.Red("!!%s not Exist.\n!!", *outputFolder)
			os.Exit(1)
		}
	}
}

func main() {

	prepare()

	tk, err := NewTokenizer(*whereStr)
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	tk.Display()

	ddb, err := NewDDB(*region, *accessKey, *secretKey, tk.TableName)
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	scanner := NewScanner(tk, ddb, *segment, *outputFolder, *delete, *limit)
	scanner.ParallelScan()
}
