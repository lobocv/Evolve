#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
Update a simple plot as rapidly as possible to measure speed.
"""


from pyqtgraph.Qt import QtGui, QtCore
import numpy as np
import pyqtgraph as pg
from pyqtgraph.ptime import time
import zmq
import json

app = QtGui.QApplication([])

p = pg.plot()
p.setWindowTitle('Evolve Statistics')
p.setRange(QtCore.QRectF(0, -10, 5000, 20)) 
p.setLabel('left', 'Number of Creatures', units='')
p.setLabel('bottom', 'Epoch', units='')
alive_line = p.plot()
deceased_line = p.plot()


# Socket to talk to server
port = "12346"
context = zmq.Context()
socket = context.socket(zmq.SUB)

print ("Collecting updates from weather server...")
socket.connect ("tcp://0.0.0.0:%s" % port)

topicfilter = b""
socket.setsockopt(zmq.SUBSCRIBE, topicfilter)

alive_data = []
deceased_data = []

loop = 0
def update():
    global alive_line, deceased_line, loop
    payload = socket.recv()
    data = json.loads(payload.decode())

    speciesname = list(data.keys())[0]
    species = data[speciesname]
    alive = species["alive"]
    alive_data.append(alive)
    deceased_data.append(species["deceased"])
    alive_line.setData(alive_data)
    deceased_line.setData(deceased_data)
    app.processEvents()  ## force complete redraw for every plot
    loop += 1
timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(0)
    


## Start Qt event loop unless running in interactive mode.
if __name__ == '__main__':
    import sys
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()

