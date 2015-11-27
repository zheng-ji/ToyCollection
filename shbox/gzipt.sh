#!/bin/bash

months=(08 09 10 11)
archive_path=s3://xxx
for month in ${months[@]}; do
	dates=`aws s3 ls $archive_path | grep 2015-$month | awk '{print $2}'`
	for that_date in $dates; do
		date_path=${archive_path}${that_date}
		list=`aws s3 ls --recursive $date_path | awk '{print $4}'`
		for path in $list; do
			path="s3:/xxxx/${path}"
			echo $path
			aws s3 cp $path tmp.gz
			if gzip -t tmp.gz ; then
				echo "success"
				echo "good $path" >> good.txt
			else
				echo "bad"
				echo "bad $path" >> bad.txt
			fi
		done
	done
done


