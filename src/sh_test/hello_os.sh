#!/bin/bash
head -n 8 $1 | tail -n 1 > $2
head -n 32 $1 | tail -n 1 >> $2
head -n 128 $1 | tail -n 1 >> $2
head -n 512 $1 | tail -n 1 >> $2
head -n 1024 $1 | tail -n 1 >> $2
# sed -n "5p" file
