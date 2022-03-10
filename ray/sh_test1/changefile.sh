#!/bin/bash
a=1
while [ $a -le 100 ]
do
	if [ $a -gt 70 ]	   #if loop variable is greater than 70
	then
		final_file_name="file"$((a))
		rm -r $final_file_name
	elif [ $a -gt 40 ]         # else if loop variable is great than 40   
	then
		final_file_name="newfile"$((a))
		origin_file_name="file"$((a))
		mv $origin_file_name $final_file_name
	fi
	           #don't forget change the loop variable
		 a=`expr  $a + 1`
done
