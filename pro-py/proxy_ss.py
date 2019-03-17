import socket, sys
from  thread import *

port=18080
maxConnection = 5
bufferSize = 4096 * 2

def start():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind('', port)
    s.listen(maxConnection)
    while True:
        try:
            conn, addr = s.accept()
            data = conn.recv(bufferSize)
            start_new_thread(conn_string, (conn, data, addr))
        except KeyboardInterrupt:
            s.close()
            sys.exit();
    s.close();
