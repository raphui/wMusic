#!/usr/bin/python
import socket
import sys

HOST = '127.0.0.1'
PORT = 1338

login = 'ACCOUNT#LOGIN#othane%TestRaphio'
load = 'STREAMER#LOAD#spotify:track:7ks6AZmFcm3Y6PGGxGSmlB%stream1'
play = 'STREAMER#PLAY#stream1'
pause = 'STREAMER#PAUSE#stream1'

s = socket.socket( socket.AF_INET , socket.SOCK_STREAM )

s.connect( ( HOST , PORT ) )

if sys.argv[1] == 'login':
	s.sendall( ( login ) )
	print 'Sended' , login
elif sys.argv[1] == 'load':
	s.sendall( ( load ) )
	print 'Sended' , load
elif sys.argv[1] == 'play':
	s.sendall( ( play ) )
	print 'Sended' , play
elif sys.argv[1] == 'pause':
	s.sendall( ( pause ) )
	print 'Sended' , pause
else:
	print "Command not supported"

#s.sendall( ( sys.argv[1] ) )

s.close()

#print 'Sended' , sys.argv[1]
