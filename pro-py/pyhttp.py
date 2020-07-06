import os
import socketserver
import http.server
import sys

# DIR = r'D:\zHome\srv'
# DIR = r'D:\zHome'
DIR = r'.'

PORT = 8080 if (len(sys.argv) < 2 or type(sys.argv[1]) == int) else int(sys.argv[1])
print("serving at port %d"%(PORT))

os.chdir(DIR)

Handler = http.server.SimpleHTTPRequestHandler
httpd = socketserver.TCPServer(("", PORT), Handler)

httpd.serve_forever()
