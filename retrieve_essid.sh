#!/bin/bash

echo $(iwlist wlan0 scan | awk '/ESSID:/ { print substr( $1 , 7 , 30 ) }')
