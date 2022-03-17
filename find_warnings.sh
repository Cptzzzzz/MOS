#!/bin/bash

gcc -Wall $1 -o test 2> warning.txt
t=$?
grep warning warning.txt | sed 's/warning: //g' > result.txt
#echo $t
if (($?))
then

	#echo "haha" >> result.txt
	n=1
	while((n<=$2))
	do
		echo $n > tem
		./test < tem >> result.txt
		let "n++"
	done
fi
pwd >> result.txt

	

