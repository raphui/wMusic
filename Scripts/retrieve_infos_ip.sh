#!/bin/bash

ipLocal=$(ifconfig wlan0 | grep 'inet addr:' | awk '{print substr($2,6,14)}')
ipPublic=$(curl -s checkip.dyndns.org | grep -Eo '[0-9\.]+')

echo $ipLocal
echo $ipPublic
