#!/bin/bash
essidU=$1
keyU=$2
ifconfig wlan0 down
iwconfig wlan0 channel 4
iwconfig wlan0 mode ad-hoc
iwconfig wlan0 essid $essidU
iwconfig wlan0 key s:azert
ifconfig wlan0 192.168.1.1
ifconfig wlan0 up

