# -*- coding: utf-8 -*-
"""
Created on Thu Jul  5 11:06:24 2018

@author: A1-0560
"""

from http.server import HTTPServer, BaseHTTPRequestHandler
import threading
import queue
import json

import time

ip='127.0.0.1'
port=9090

# Factory Style
def TqThreadWorker(iQueue):
    class mThreadWorkerClass(threading.Thread):
        def __init__(self, *args, **kwargs):
            super(mThreadWorkerClass, self).__init__(*args, **kwargs)
            self.mQueue = iQueue
            self.bStop = False # stop bit
            self.currentTask = None

        def run(self):
            while not self.bStop:
                try:
                    task = self.mQueue.get(block=True, timeout = None)
                except self.mQueue.Empty:
                    continue
                # task begins
                self.currentTask = task
                
                # do task
                for i in range(4):
                    print("task message %d: %s" % (i, task[0]))
                    time.sleep(1)
                
                
                # task ends
                self.currentTask = None
                self.mQueue.task_done()
                
        def stop(self):
            # stop from block  vs stop from working
            self.bStop = True    
            
        def put(self, params = []):
            self.mQueue.put(params, block=True, timeout=None)
        
        @property
        def getSize(self):
            return self.mQueue.qsize()
            
        @property
        def getCurrentTask(self):
            return self.currentTask

    return mThreadWorkerClass

def TqHttpHandler(iThreadWorker):
    class mHttpHandlerClass(BaseHTTPRequestHandler):
        def __init__(self, *args, **kwargs):
            self.mThreadWorker = iThreadWorker
            # super method must place at last
            super(mHttpHandlerClass, self).__init__(*args, **kwargs)
            
        def do_POST(self):
            content_len = int(self.headers['Content-Length'])
            request_body = self.rfile.read(content_len)
            mData = json.loads(request_body)
            # print(mData)
            message = mData.get('message')
            if message != None:
                self.send_response(200)
                self.end_headers()
                self.mThreadWorker.put([message])
                self.wfile.write(str('succeeded request').encode('utf-8'))
            else:
                self.send_response(400)
                self.end_headers()
                self.wfile.write(str('failed request').encode('utf-8'))

        def do_GET(self):
            self.send_response(200)
            self.end_headers()
            self.wfile.write(str(self.mThreadWorker.getSize).encode('utf-8'))
            
    return mHttpHandlerClass

taskQueue = queue.Queue(10)
mWorker = TqThreadWorker(taskQueue)()
mServer = HTTPServer((ip, port), TqHttpHandler(mWorker))

print('ip and port =>  %s:%d'%(ip, port))
print('==> http-post {"message":"value"}, then print "value" 4 times')
print('==> only one thread can print, others will be blocked')
print('==> http-get return the number of blocked-thread')
mWorker.start()
mServer.serve_forever()