#!/bin/bash

COUNT=0

while [  $COUNT -lt 5000000 ]; do
  ./ASLR_linux.out
  let COUNT=COUNT+1 

  if test "$((COUNT%1000))" = 0; then
    echo "Progress: $COUNT out of 5000000";
  fi

done
