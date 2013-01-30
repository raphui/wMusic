#!/usr/bin/python
import socket
from subprocess import Popen
from subprocess import PIPE

#call("/home/raphio/Documents/Development/SpotifyEverywhere/Scripts/retrieve_essid.sh" , shell=True )

pipe = Popen("/home/raphio/Documents/Development/SpotifyEverywhere/Scripts/retrieve_essid.sh" , shell=True , bufsize=1024).stdout
