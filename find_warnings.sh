#!/bin/bash
gcc -Wall $1 -o test 2> warning.txt

grep warning warning.txt | grep s/warning: //g >result.txt

if(($?))
do

	n=0
	while((n<$2))
	do
		./test >> result.txt
		echo $n
		let "n++"
	done
done
pwd >> result.txt

	
