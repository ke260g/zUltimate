#!/bin/sh
PROG_NAME=falcon
while 1; do
   pid=`ps l | grep $PROG_NAME | grep -v grep | awk '{print $3}' | head -1`
   if [ -z $pid ]; then
      sleep 1
      continue
   fi
   strace -p $pid
done