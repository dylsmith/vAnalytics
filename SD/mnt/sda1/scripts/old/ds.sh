#!/bin/ash
rsync --remove-source-files --exclude Placeholder.txt -r -e \
` /mnt/sd/scripts/serverConnect.sh `:~/data/ &>/dev/null 
