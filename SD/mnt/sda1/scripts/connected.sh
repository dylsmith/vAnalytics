#!/bin/ash
while [ 1 ]; do
  #silent ping, 1 packet, 8 bytes, 2 second tiemout 
  ping -c 1 -s 8 -W 2 google.com > /dev/null  #1 ping, 8 bytes, 2 second timeout
  if [[ $? -eq 0 ]]; then	  
    touch /mnt/sd/connected
    #echo connected
  else
    if [ -e /mnt/sd/connected ]; then
      rm /mnt/sd/connected
    fi
    #echo disconnected
  fi
  sleep 3
done

