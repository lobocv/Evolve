#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
Update a simple plot as rapidly as possible to measure speed.
"""


from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg
import zmq
import json


def create_subplot(title='', xlabels=None):
    global layout
    axis = PhenotypeAxis(xlabels, orientation='bottom')

    plot = layout.addPlot(title=title, axisItems={'bottom': axis})
    plot.showGrid(x=True, y=True, alpha=0.6)
    plot.setWindowTitle(title)
    plot.setLabel('left', 'Number of Creatures', units='')
    plot.setLabel('bottom', 'Epoch', units='')

    return plot


def update():
    global plots, view, layout, loop
    max_row = 3
    payload = socket.recv()
    data = json.loads(payload.decode())

    spacing = 0
    barwidth = 1

    for speciesname, species_info in data.items():
        if speciesname not in plots:
            plots[speciesname] = {}

        for ii, (traitname, trait) in enumerate(species_info["phenotypes"].items()):
            if traitname not in plots[speciesname]:
                plot = create_subplot(title="%s: %s" % (speciesname, traitname), xlabels=list(trait.keys()))
                plot.setRange(QtCore.QRectF(0, 0, len(trait) + len(trait)*spacing, 500))
                plots[speciesname][traitname] = {}

            if (ii+1) % max_row == 0 and ii > 0:
                layout.nextRow()

            for jj, (phenotype, phenotype_count) in enumerate(trait.items()):
                if phenotype not in plots[speciesname][traitname]:

                    bar = pg.BarGraphItem(x=[jj + jj*spacing], height=phenotype_count, width=barwidth)
                    plot.addItem(bar)
                    plots[speciesname][traitname][phenotype] = bar
                else:
                    bar = plots[speciesname][traitname][phenotype]
                    bar.setOpts(height=phenotype_count)

    # force complete redraw for every plot
    app.processEvents()


class PhenotypeAxis(pg.AxisItem):

    def __init__(self, x_axis_labels, *args, **kwargs):
        super(PhenotypeAxis, self).__init__(*args, **kwargs)
        self.x_axis_labels = x_axis_labels

    def tickStrings(self, values, scale, spacing):

        # return list(range(len(self.x_axis_labels))), self.x_axis_labels
        # return (map(str, values)), self.x_axis_labels + list(map(str, values))
        strns = []

        for x in values:
            xint = int(round(x))
            try:
                s = self.x_axis_labels[xint]
            except IndexError:
                s = str(x)
            # s = str(x)
            strns.append(s)

        return strns

# Start Qt event loop unless running in interactive mode.
if __name__ == '__main__':
    import sys

    app = QtGui.QApplication([])
    view = pg.GraphicsView()
    layout = pg.GraphicsLayout()
    view.setCentralItem(layout)
    view.show()

    print("Collecting phenotype statistics from Evolve server...")
    # Socket to talk to server
    port = "12346"
    context = zmq.Context()
    socket = context.socket(zmq.SUB)
    socket.connect("tcp://0.0.0.0:%s" % port)

    topicfilter = b""
    socket.setsockopt(zmq.SUBSCRIBE, topicfilter)

    plots = {}

    timer = QtCore.QTimer()
    timer.timeout.connect(update)
    timer.start(0)

    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()

