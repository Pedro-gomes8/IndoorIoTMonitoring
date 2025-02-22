#!/bin/bash


while true; do
  echo "Hello Arduino" > /dev/tty.HC-05
  sleep 0.1  # 100ms delay
done
