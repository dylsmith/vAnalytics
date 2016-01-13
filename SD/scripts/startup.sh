#!/bin/ash
if ! pidof "connected.sh" > /dev/null; then
  /mnt/sd/scripts/connected.sh &
fi
if ! pidof "dataSync.sh" > /dev/null; then
  /mnt/sd/scripts/dataSync.sh &
fi
