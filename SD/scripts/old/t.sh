#!/bin/ash

while true; do
  wget -q --tries=10 --timeout=20 --spider google.com
  if [[ $? -eq 0 ]];then
    touch /mnt/sd/connected
    if [-e /mnt/sd/VIChanged ]; then
      scp /mnt/sd/VI.txt (server):/tmp/VI.txt
      ssh (server) 'mv /tmp/VI.txt /VI.txt'
    fi
    for file in /mnt/sd/data/* do
      scp file (server):/tmp/file
      ssh (server) 'mv /tmp/file /data/file'
      rm file
    done
  else
    rm /mnt/sd/connected 
  fi
  sleep 1
done
