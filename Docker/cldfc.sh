#!/bin/bash

# Usage:
# Go into cmd loop: sudo ./cldfc.sh
# Run single cmd:  sudo ./cldfc.sh <cldfc paramers>

PREFIX="docker-compose exec noddfcd cldfc"
if [ -z $1 ] ; then
  while :
  do
    read -e -p "cldfc " cmd
    history -s "$cmd"
    $PREFIX $cmd
  done
else
  $PREFIX "$@"
fi
