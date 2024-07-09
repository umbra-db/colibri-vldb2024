from typing import Dict, Set, Any

import matplotlib
from matplotlib.artist import Artist
from matplotlib.patches import Rectangle

from plot.color import Color
from plot.frame import Frame
from plot.pattern import Pattern
from plot.plot import Plot


class BarPlot(Plot):

    def __init__(self, frame: Frame, width: float = 0.8, color: Color = Color.WHITE):
        super().__init__(frame)
        self.width = width
        self.color = color

        matplotlib.rcParams['hatch.linewidth'] = 0.5
        self.edgewidth = 0.5
        self.bars = []

    def plot(self, data: [[float]], colors: [Color] = None, patterns: [Pattern] = None) -> [Artist]:
        if len(data) == 0:
            return

        ids: Set[int] = set()

        for x, bars in enumerate(data):
            num_bars = len([b for b in bars if b is not None])
            bar_width = self.width / num_bars
            self.bars.append([])

            count = 0
            for i, y in enumerate(bars):
                if y is None:
                    continue
                ids.add(i)

                color = self.color if colors is None else colors[i]
                pattern = None if patterns is None else patterns[i].value
                self.bars[-1].append(self.ax.bar([x + (bar_width - self.width) * num_bars / (2 * num_bars) + count * bar_width], [y], bar_width, color=color, hatch=pattern,
                                                 zorder=10, edgecolor=Color.DARKGRAY, linewidth=self.edgewidth))
                count += 1

        self.ax.axhline(y=self.ax.get_ylim()[0], color='k', linestyle='-', linewidth=0.5, zorder=20)

        handles = []
        for i in sorted(ids):
            color = self.color if colors is None else colors[i]
            pattern = None if patterns is None else patterns[i]
            handles.append(Rectangle((0, 0), 1, 1, facecolor=color, hatch=pattern, edgecolor=Color.BLACK, linewidth=self.edgewidth))

        return handles

    def plotInfo(self, data: [[(float, Any)]], colors: Dict[Any, Color] = None, patterns: Dict[Any, Pattern] = None) -> Dict[Any, Artist]:
        if len(data) == 0:
            return

        ids: Set[Any] = set()

        for x, bars in enumerate(data):
            bar_width = self.width / len(bars)
            self.bars.append([])

            for i, (y, info) in enumerate(bars):
                ids.add(info)

                color = self.color if colors is None else colors[info]
                pattern = None if patterns is None else patterns[info]
                self.bars[-1].append(self.ax.bar([x + 0.5 + (1 - self.width + bar_width) / 2 + i * bar_width], [y - self.yformatter.baseline()], self.width / len(bars), color=color, hatch=pattern,
                                                 zorder=10, edgecolor=Color.BLACK, linewidth=self.edgewidth, bottom=self.yformatter.baseline()))

        self.ax.axhline(y=self.ax.get_ylim()[0], color='k', linestyle='-', linewidth=0.5, zorder=20)

        handles = {}
        for i in sorted(ids):
            color = self.color if colors is None else colors[i]
            pattern = None if patterns is None else patterns[i]
            handles[i] = Rectangle((0, 0), 1, 1, facecolor=color, hatch=pattern, edgecolor=Color.BLACK, linewidth=self.edgewidth)

        return handles

    def plotStacked(self, data: [[[float]]], colors: [[Color]] = None, patterns: [[Pattern]] = None) -> [Artist]:
        if len(data) == 0:
            return

        ids: Dict[int, Set[int]] = {}

        for x, bars in enumerate(data):
            bar_width = self.width / len(bars)

            for i, bar in enumerate(bars):
                if i not in ids:
                    ids[i] = set()

                bottom = 0
                for j, y in enumerate(bar):
                    ids[i].add(j)
                    color = self.color if colors is None else colors[i][j]
                    pattern = None if patterns is None else patterns[i][j]
                    self.ax.bar([x + 0.5 + (1 - self.width + bar_width) / 2 + i * bar_width], [y - bottom], self.width / len(bars), bottom=bottom, color=color, hatch=pattern, zorder=10,
                                edgecolor=Color.BLACK, linewidth=self.edgewidth)
                    bottom += y

        self.ax.axhline(y=0, color='k', linestyle='-', linewidth=1, zorder=20)

        handles = []
        for i in sorted(ids.keys()):
            for j in sorted(ids[i]):
                color = self.color if colors is None else colors[i][j]
                pattern = None if patterns is None else patterns[i][j]
                handles.append(Rectangle((0, 0), 1, 1, facecolor=color, hatch=pattern, edgecolor=Color.BLACK, linewidth=self.edgewidth))

        return handles

    def plotDifference(self, difference: [str]):
        for (i, b) in enumerate(self.bars):
            assert len(b) == 2
            bar0, bar1 = b[0][0], b[1][0]
            if bar0.get_height() <= bar1.get_height():
                middle = bar0.get_x() + bar0.get_width() / 2
            else:
                middle = bar1.get_x() + bar1.get_width() / 2
            height = max(bar0.get_height(), bar1.get_height())
            self.ax.plot([bar0.get_x(), bar0.get_x() + bar0.get_width() * 2], [height, height], 'k-', lw=0.5, zorder=20)
            self.ax.plot([middle, middle], [bar0.get_height(), bar1.get_height()], 'k-', lw=0.5, zorder=20)
            self.ax.text(bar1.get_x(), height + self.ax.get_ylim()[1] * 0.01, difference[i], ha='center', va='bottom', fontsize=7, color='k')
