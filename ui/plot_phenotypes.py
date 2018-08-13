#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
Update a simple plot as rapidly as possible to measure speed.
"""
import json

import pyqtgraph as pg
import zmq
from pyqtgraph.Qt import QtGui, QtCore


def create_subplot(title='', xlabels=None, spacing=3):
    global layout
    axis = PhenotypeAxis(xlabels, orientation='bottom')
    axis.setTickSpacing(major=spacing, minor=spacing,)

    plot = layout.addPlot(title=title, axisItems={'bottom': axis})
    plot.showGrid(x=True, y=True, alpha=0.6)
    plot.setWindowTitle(title)
    plot.setLabel('left', 'Number of Creatures', units='')
    plot.setLabel('bottom', 'Phenotype', units='')

    vb = plot.getViewBox()
    vb.setLimits(xMin=-1, yMin=0)
    return plot


@QtCore.pyqtSlot()
def update_plots(data):
    global plots, view, layout
    spacing = 5
    barwidth = 1
    max_row = 3

    for speciesname, species_info in data.items():
        if speciesname not in plots:
            plots[speciesname] = {}

        if "phenotypes" not in species_info:
            continue

        for ii, (traitname, trait) in enumerate(species_info["phenotypes"].items()):
            if traitname not in plots[speciesname]:
                plot = create_subplot(title="%s: %s" % (speciesname, traitname), xlabels=list(trait.keys()), spacing=spacing)
                plot.setRange(QtCore.QRectF(0, 0, len(trait) + len(trait)*spacing, 500))
                plots[speciesname][traitname] = {}

            if (ii+1) % max_row == 0 and ii > 0:
                layout.nextRow()

            colors = 'bgrcmykw'
            for jj, (phenotype, phenotype_count) in enumerate(trait.items()):
                if phenotype not in plots[speciesname][traitname]:
                    bar = pg.BarGraphItem(x=[jj*spacing], height=phenotype_count, width=barwidth, brush=colors[jj])
                    plot.addItem(bar)
                    plots[speciesname][traitname][phenotype] = (plot, bar)
                else:
                    plot, bar = plots[speciesname][traitname][phenotype]
                    bar.setOpts(height=phenotype_count)


class PhenotypeAxis(pg.AxisItem):

    def __init__(self, x_axis_labels, *args, **kwargs):
        super(PhenotypeAxis, self).__init__(*args, **kwargs)
        self.x_axis_labels = x_axis_labels

    def tickStrings(self, values, scale, spacing):
        """Override this method to replace axis tick numbers with phenotype labels. """
        strns = []

        for x in values:
            x /= float(spacing)
            try:
                if x >= 0 and x % 1 < 0.01:
                    x = int(round(x))
                    s = self.x_axis_labels[x]
                else:
                    s = ""
            except IndexError:
                s = ""
            strns.append(s)

        return strns

    def generateDrawSpecs(self, p):
        """ Override this method to alternate the y position of tick labels so that they don't overlap."""
        axisSpec, tickSpecs, textSpecs = super(PhenotypeAxis, self).generateDrawSpecs(p)

        do_shift = True
        for rect, textflags, vstr in textSpecs:
            if vstr:
                shift = rect.height()*1.5
                if do_shift:
                    rect.adjust(0, 0, shift, shift)
                do_shift = not do_shift

        return axisSpec, tickSpecs, textSpecs


class MessageQueueThread(QtCore.QThread):

    update = QtCore.pyqtSignal(dict)

    def run(self):
        while 1:
            payload = socket.recv()
            data = json.loads(payload.decode())
            self.update.emit(data)


# Start Qt event loop unless running in interactive mode.
if __name__ == '__main__':
    import sys

    app = QtGui.QApplication([])
    view = pg.GraphicsView()
    layout = pg.GraphicsLayout()
    view.setCentralItem(layout)
    view.showMaximized()

    print("Collecting phenotype statistics from Evolve server...")
    # Socket to talk to server
    port = "12346"
    context = zmq.Context()
    socket = context.socket(zmq.SUB)
    socket.connect("tcp://0.0.0.0:%s" % port)

    topicfilter = b""
    socket.setsockopt(zmq.SUBSCRIBE, topicfilter)

    plots = {}

    t = MessageQueueThread()
    t.update.connect(update_plots)
    t.start()

    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()

