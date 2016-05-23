#!/bin/ash
rsync --remove-source-files --exclude Placeholder.txt -r -e 'ssh -i /mnt/sd/keys/privatekey' /mnt/sd/data/ \
  ` cat /mnt/sd/scripts/util/amazoninstance `:~/data/ &>/dev/null 
