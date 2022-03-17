#!/bin/bash

gcc -Wall $1 -o test 2> warning.txt

grep warning warning.txt | sed 's/warning: //g' > result.txt


if(($?))
then

	n=0
	while((n<$2))
	do
		./test >> result.txt
		let "n++"
		echo $n
		
	done
fi
pwd >> result.txt

	
