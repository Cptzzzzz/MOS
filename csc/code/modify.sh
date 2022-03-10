#!/bin/bash
#$1 file name  $2 origin text $3 after text
sed -i s/$2/$3/g $1 
