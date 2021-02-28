#!/bin/bash

make clean > /dev/null && make > /dev/null

#Daemonize - run in the background!
./dl_demo &
#process id
pid=$!
sleep 1

echo "Bash script: Copying my_dl2.so to my_dl.so."

cp my_dl2.so my_dl.so

wait $pid
#Wait for pid to finish before we exit!