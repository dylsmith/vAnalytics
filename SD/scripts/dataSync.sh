#!/bin/ash
while [ 1 ]; do
    rsync --remove-source-files -r -e "ssh -i /mnt/sd/keys/private" /mnt/sd/data/ \
    ` cat /mnt/sd/scripts/util/amazoninstance `:~/data/ &> /dev/null 
  sleep 5
done
