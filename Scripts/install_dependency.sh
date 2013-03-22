#!/bin/bash

#Get libspotify
wget https://developer.spotify.com/download/libspotify/libspotify-12.1.103-Linux-armv6-bcm2708hardfp-release.tar.gz
tar -xvzf libspotify-12.1.103-Linux-armv6-bcm2708hardfp-release.tar.gz
cd libspotify-12.1.103-Linux-armv6-bcm2708hardfp-release
sudo make install prefix=/usr/local
echo "LibSpotify has been installed"

#Get vlc
sudo apt-get install libvlc-dev
echo "LibVlc has been installed"

#Get libasound2-dev
sudo apt-get install libasound2-dev
echo "LibAsound2 has been installed"

#Get node.js
sudo apt-get install nodejs nodejs-dev
echo "Node.js has been installed"

echo "All dependency has been installed"
