import os
import socketserver
import http.server

PORT = 8080
# DIR = r'D:\zHome\srv'
# DIR = r'D:\zHome'
DIR = r'E:\sdk\trunk\Sdk_Development_Code\qdazzle_h5sdk\debug_demo\dist'

print("serving at port %d", PORT)

os.chdir(DIR)

Handler = http.server.SimpleHTTPRequestHandler
httpd = socketserver.TCPServer(("", PORT), Handler)

httpd.serve_forever()
