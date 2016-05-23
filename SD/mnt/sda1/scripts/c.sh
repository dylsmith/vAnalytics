#!/bin/ash

while [ 1 ]; do
  ping -c 1 -s 8 -W 2 google.com > /dev/null  #1 ping, 8 bytes, 2 second timeout
  if [[ $? -eq 0 ]]; then
    touch /mnt/sd/flags/connected
    #echo connected
  else
    #echo disconnected
    if [ -e /mnt/sd/flags/connected ]; then
      rm /mnt/sd/connected
    fi
    echo killing wifi
    wifi down &> /dev/null
    echo starting wifi
    wifi &> /dev/null
    echo sleeping for 30
    sleep 30
    echo done sleeping
  fi
  sleep 2
done


