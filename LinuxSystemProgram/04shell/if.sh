#!/bin/bash
#this is a comment line
echo "Enter your filename:"
read myfile
if [ -e $myfile ];then
    if [ -s $myfile ];then
        echo "$myfile exist and size greater than zero"
    else
        echo "$myfile exist but size is zero"
    fi
else
    echo "file no exist"
fi










