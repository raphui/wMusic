#!/bin/bash

#Get libspotify
wget https://developer.spotify.com/download/libspotify/libspotify-12.1.103-Linux-armv6-bcm2708hardfp-release.tar.gz
tar -xvzf libspotify-12.1.103-Linux-armv6-bcm2708hardfp-release.tar.gz
cd libspotify-12.1.103-Linux-armv6-bcm2708hardfp-release
sudo make install prefix=/usr/local
echo "\e[01;33mLibSpotify has been installed\e[00m"

#Get vlc
sudo apt-get install libvlc-dev
echo "\e[01;33mLibVlc has been installed\e[00m"

#Get libasound2-dev
sudo apt-get install libasound2-dev
echo "\e[01;33mLibAsound2 has been installed\e[00m"

#Get node.js
sudo apt-get install nodejs nodejs-dev
echo "\e[01;33mNode.js has been installed\e[00m"

echo "\e[01;33mAll dependency has been installed\e[00m"
