#!/bin/ash
rsync --remove-source-files --exclude *.placeholder -r -e 'ssh -i /mnt/sd/keys/privatekey' /mnt/sd/data/complete/ \
  ` cat /mnt/sd/scripts/util/amazoninstance `:~/data/ &>/dev/null 
