#!/bin/ash
rsync --remove-source-files -r -e "ssh -i /mnt/sd/keys/private" $1 \
` cat /mnt/sd/scripts/util/amazoninstance `:~/` basename $1 `
