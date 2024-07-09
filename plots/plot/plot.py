from enum import Enum

from matplotlib import pyplot as plt
import numpy as np
from matplotlib.artist import Artist
from matplotlib.ticker import LogLocator, NullFormatter, AutoMinorLocator

from plot.formatter import Formatter, Grid, Decorator
from plot.frame import Frame
from plot.location import Location


class Plot:

    def __init__(self, frame: Frame):
        self.frame = frame
        self.ax = frame.get_axes()
        self.xformatter = Formatter()
        self.yformatter = Formatter()

    def change_frame(self, frame: Frame):
        self.frame = frame
        self.ax = frame.get_axes()
        self.xformatter = Formatter()
        self.yformatter = Formatter()

    def format_axes(self, xformatter: Formatter, yformatter: Formatter):
        self.xformatter = xformatter
        self.yformatter = yformatter

        spine_color = 'black'

        for spine in ['left', 'bottom', 'top', 'right']:
            self.ax.spines[spine].set_color(spine_color)
            self.ax.spines[spine].set_linewidth(0.5)

        self.ax.set_xscale(xformatter.scale())
        self.ax.set_yscale(yformatter.scale())

        self.ax.yaxis.set_ticks_position('left')
        self.ax.yaxis.set_tick_params(direction='out', color=spine_color)
        #self.ax.tick_params(axis='y', which='major', pad=0.5)

        if yformatter.scale() == 'log':
            locmaj = LogLocator(base=10, numticks=12)
            self.ax.yaxis.set_major_locator(locmaj)
            self.ax.yaxis.set_major_formatter(yformatter)
            locmin = LogLocator(base=10.0, subs=(np.arange(0, 1, 0.1)), numticks=12)
            self.ax.yaxis.set_minor_locator(locmin)
            self.ax.yaxis.set_minor_formatter(NullFormatter())
        else:
            self.ax.yaxis.set_major_formatter(yformatter)
            if yformatter.ticks() is not None:
                self.ax.set_yticks(yformatter.ticks())

            if yformatter.grid() == Grid.MINOR:
                self.ax.yaxis.set_minor_locator(AutoMinorLocator(n=2))
                self.ax.yaxis.grid(visible=True, which='minor', linewidth=0.5, linestyle=':')

        if yformatter.label() is not None:
            self.ax.set_ylabel(yformatter.label())

        if yformatter.grid() >= Grid.MAJOR:
            self.ax.yaxis.grid(visible=True, which='major', linewidth=0.5)

        if yformatter.decorator() <= Decorator.NONE:
            self.ax.tick_params(axis="y", which='both', left=False)
        if yformatter.decorator() <= Decorator.TICKS:
            self.ax.yaxis.set_ticklabels([])
        if yformatter.decorator() <= Decorator.TICKLABELS:
            self.ax.set_ylabel(None)

        self.ax.xaxis.set_ticks_position('bottom')
        self.ax.xaxis.set_tick_params(direction='out', color=spine_color)
        self.ax.tick_params(axis='x', which='major', pad=0.5)

        if xformatter.scale() == 'log':
            if xformatter.ticks() is not None:
                self.ax.set_xticks(xformatter.ticks())
            else:
                locmaj = LogLocator(base=10, numticks=12)
                self.ax.xaxis.set_major_locator(locmaj)
            self.ax.xaxis.set_major_formatter(xformatter)
            locmin = LogLocator(base=10.0, subs=(np.arange(0, 1, 0.1)), numticks=12)
            self.ax.xaxis.set_minor_locator(locmin)
            self.ax.xaxis.set_minor_formatter(NullFormatter())
        else:
            self.ax.xaxis.set_major_formatter(xformatter)
            if xformatter.ticks() is not None:
                self.ax.set_xticks(xformatter.ticks())

            if xformatter.grid() == Grid.MINOR:
                self.ax.xaxis.set_minor_locator(AutoMinorLocator(n=2))
                self.ax.xaxis.grid(visible=True, which='minor', linewidth=0.5, linestyle=':')

        if xformatter.label() is not None:
            self.ax.set_xlabel(xformatter.label())

        if xformatter.grid() >= Grid.MAJOR:
            self.ax.xaxis.grid(visible=True, which='major', linewidth=0.5)

        if xformatter.decorator() <= Decorator.NONE:
            self.ax.tick_params(axis="x", which='both', bottom=False)
        if xformatter.decorator() <= Decorator.TICKS:
            self.ax.xaxis.set_ticklabels([])
        if xformatter.decorator() <= Decorator.TICKLABELS:
            self.ax.set_xlabel(None)

        # https://stackoverflow.com/questions/65243861/matplotlib-python-y-axis-labels-not-aligned-in-pgf-format
        for lab in self.ax.yaxis.get_ticklabels():
            lab.set_verticalalignment("center")

        # Set limits for axis
        if yformatter.limit() is not None:
            (bottom, top) = yformatter.limit()
            self.ax.set_ylim(bottom=bottom, top=top)
        if xformatter.limit() is not None:
            (left, right) = xformatter.limit()
            self.ax.set_xlim(left=left, right=right)

        if xformatter.rotation() is not None:
            for label in self.ax.get_xticklabels():
                if xformatter.rotation() not in [0, 90, 180, 270]:
                    label.set_ha("right")
                label.set_rotation(xformatter.rotation())
                label.set_rotation_mode("anchor")
        if yformatter.rotation() is not None:
            for label in self.ax.get_yticklabels():
                if yformatter.rotation() not in [0, 90, 180, 270]:
                    label.set_ha("right")
                label.set_rotation(yformatter.rotation())
                label.set_rotation_mode("anchor")

        yformatter.format_axes(self.ax)
        xformatter.format_axes(self.ax)

    def title(self, text: str, pad=5, fontsize=plt.rcParams['axes.titlesize']):
        fontdict = {'fontsize': fontsize}
        self.ax.set_title(text, pad=pad, fontdict=fontdict)

    def legend(self, handles: [Artist], labels: [str], columns: int = 1, bbox_to_anchor: (float, float) = (1, 0.5), location: Location = Location.BEST, labelspacing=0.5,
               handlelength: float = None, handleheight: float = None):
        assert len(handles) == len(labels)
        legend = self.ax.legend(handles, labels, ncol=columns, bbox_to_anchor=bbox_to_anchor, loc=location, frameon=False, framealpha=1, labelspacing=labelspacing, handlelength=handlelength,
                                handleheight=handleheight)
        self.ax.add_artist(legend)
