#!/bin/bash

COUNT=0

while [  $COUNT -lt 100 ]; do
  ./ASLR_linux.out
  let COUNT=COUNT+1 

  #if test "$((COUNT%10000))" = 0; then
  #  echo "Progress: $COUNT out of 10000000";
  #fi

done

size=`wc -l /root/ASLR.csv | awk '{print $1}'`;

if [ $size -lt 500 ]; then
	/sbin/shutdown -h now
fi