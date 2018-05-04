#!/usr/bin/python

# server code
import socket
import Tkinter as tk

HOST='192.168.1.159'
PORT=5002
s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
conn, addr=s.accept()
s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
print 'Connected by', addr

root = tk.Tk()
frame = tk.Frame(root)
frame.pack()

def send_forward():
    conn.send('1')
    print('sent: go')

def send_stop():
    conn.send('2')
    print('sent: stop')

def send_kill():
    conn.send('3')
    print('sent: kill command')

forward = tk.Button(frame,
                    text="START",
                    command=send_forward)

forward.pack(side=tk.LEFT)


stop = tk.Button(frame, 
                   text="STOP", 
                   fg="red",
                   command=send_stop)


stop.pack(side=tk.LEFT)
                 
quitButton = tk.Button(frame,
                   text="KILL SERVER",
                   command=root.destroy
                  )
quitButton.pack(side=tk.RIGHT)

kill = tk.Button(frame,text="REMOTE KILL",command=send_kill)
kill.pack(side=tk.RIGHT)
root.mainloop()

s.close()
