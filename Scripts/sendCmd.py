import socket
import sys

HOST = '127.0.0.1'
PORT = 1338

s = socket.socket( socket.AF_INET , socket.SOCK_STREAM )

s.connect( ( HOST , PORT ) )

s.sendall( ( sys.argv[1] ) )

s.close()

print 'Sended' , sys.argv[1]
