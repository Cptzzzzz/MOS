#!/bin/bash
#First you can use grep (-n) to find the number of lines of string.
#Then you can use awk to separate the answer.

grep $2 $1 -n > temp
awk -F: '{print $1}' temp > $3
rm temp
