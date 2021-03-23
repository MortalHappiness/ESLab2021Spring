#!/usr/bin/env python3
import socket
import numpy as np
import json
import time
import random
HOST = '0.0.0.0' # Standard loopback interface address
PORT = 54321 # Port to listen on (use ports > 1023)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen()
conn, addr = s.accept()
i=0
while conn:
    print('Connected by', addr)
    while True:
        data = conn.recv(1024).decode('utf-8')
        print(data)
        # if (data != ""):
        #     data = json.loads(data)
        #     print('Received from socket server : ', data)
           # data = json.loads(data)
        if (i == 0):
            conn.send(b"Hello from server");
            i+=1;
