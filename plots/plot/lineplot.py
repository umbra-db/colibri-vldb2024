from typing import List
import numpy as np
from matplotlib.artist import Artist
from matplotlib.patches import Rectangle
from matplotlib.transforms import Bbox
from mpl_toolkits.axes_grid1.inset_locator import TransformedBbox, BboxPatch, BboxConnector

from plot.color import Color
from plot.frame import Frame
from plot.linestyle import Linestyle
from plot.marker import Marker
from plot.plot import Plot
from plot.formatter import ZoomFormatter


class LinePlot(Plot):

    def __init__(self, frame: Frame, linewidth: float = 0.75, linestyle: Linestyle = Linestyle.SOLID, linecolor: Color = Color.BLACK, marker: Marker = None, markersize: float = 2.5):
        super().__init__(frame)
        self.linewidth = linewidth
        self.linestyle = linestyle
        self.linecolor = linecolor
        self.marker = marker
        self.markersize = markersize

    def plot(self, x: List[float] = None, y: List[float] = [], color: Color = None, linewidth: float = None, linestyle: Linestyle = None, marker: Marker = None, markersize: float = None) -> Artist:
        if len(y) == 0:
            return

        x = [i for i in range(len(y))] if x is None else x
        width = self.linewidth if linewidth is None else linewidth
        style = self.linestyle if linestyle is None else linestyle
        color = self.linecolor if color is None else color
        marker = self.marker if marker is None else marker
        markersize = self.markersize if markersize is None else markersize
        return self.ax.plot(x, y, linewidth=width, linestyle=style, color=color, marker=marker, markersize=markersize)[0]

    def plot_scatter(self, x: List[float] = None, y: List[float] = [], color: Color = None, marker: Marker = None, markersize: float = None) -> Artist:
        if len(y) == 0:
            return

        x = [i for i in range(len(y))] if x is None else x
        color = self.linecolor if color is None else color
        marker = self.marker if marker is None else marker
        markersize = self.markersize if markersize is None else markersize
        return self.ax.scatter(x, y, color=color, marker=marker, s=markersize, zorder=2)

    def zoom(self, position: List[float], bounds: List[float], padding = 2) -> Plot:
        assert len(position) == 4
        assert len(bounds) == 4

        p = LinePlot(self.frame, self.linewidth, self.linestyle, self.linecolor, self.marker, self.markersize)
        p.frame = None
        p.ax = self.ax.inset_axes(position, xlim=(bounds[0], bounds[2]), ylim=(bounds[1], bounds[3]))
        p.format_axes(ZoomFormatter(self.xformatter, bounds[0], bounds[0] + bounds[2]), ZoomFormatter(self.yformatter, bounds[1], bounds[1] + bounds[3]))

        def indicator(edgecolor="gray", zorder=2, linewidth=0.5, linestyle="--", alpha=1):
            x, y, width, height = bounds
            rectangle_patch = Rectangle((x, y), width, height, facecolor="none", edgecolor=edgecolor, zorder=zorder, linewidth=linewidth, linestyle=linestyle, alpha=alpha)
            self.ax.add_patch(rectangle_patch)

            # connect the inset_axes to the rectangle
            connects = []
            for (loc1, loc2) in [(1, 1), (2, 2), (4, 4), (3, 3)]:
                connection_patch = BboxConnector(p.ax.bbox, rectangle_patch, loc1=loc1, loc2=loc2, edgecolor=edgecolor, zorder=zorder, linewidth=linewidth, linestyle=linestyle, alpha=alpha)
                connects.append(connection_patch)
                self.ax.add_patch(connection_patch)

            # decide which two of the lines to keep visible....
            pos = p.ax.get_position()
            bboxins = pos.transformed(self.ax.figure.transSubfigure)
            rectbbox = Bbox.from_bounds(*position).transformed(self.ax.transData)
            x0 = rectbbox.x0 < bboxins.x0
            x1 = rectbbox.x1 < bboxins.x1
            y0 = rectbbox.y0 < bboxins.y0
            y1 = rectbbox.y1 < bboxins.y1
            connects[0].set_visible(x0 ^ y0)
            connects[1].set_visible(x0 == y1)
            connects[2].set_visible(x1 == y0)
            connects[3].set_visible(x1 ^ y1)


            padding_x = bounds[2] / (bboxins.x1 - bboxins.x0) * padding
            padding_y = bounds[3] / (bboxins.y1 - bboxins.y0) * padding
            p.ax.set_xlim(bounds[0] - padding_x, bounds[0] + bounds[2] + 2 * padding_x)
            p.ax.set_ylim(bounds[1] - padding_y, bounds[1] + bounds[3] + 2 * padding_y)

        indicator()

        return p
        