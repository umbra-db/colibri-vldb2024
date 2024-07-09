import os

from plot.color import Color
from plot.formatter import Decorator, Grid, ThroughputFormatter
from plot.frame import latexify, cm2inch, Format, SingleFrame
from plot.lineplot import LinePlot

if __name__ == "__main__":
    latexify(hyperref=False, increase_font=0)
    frame = SingleFrame(width=cm2inch(7), height=cm2inch(4))
    plot = LinePlot(frame, linewidth=2, markersize=2)
    plot.format_axes(yformatter=ThroughputFormatter(label="Analytical Performance", min=0, max=100, grid=Grid.NONE, decorator=Decorator.NONE),
                     xformatter=ThroughputFormatter(label="Transactional Performance", min=0, max=105, grid=Grid.NONE, decorator=Decorator.NONE))

    plot.ax.set_ylabel("Analytical Performance")
    plot.ax.set_xlabel("Transactional Performance")

    i = 0
    x1 = []
    y1 = []
    y2 = []
    zero = []
    while i <= 15:
        x1.append(i)
        y2.append(i * 2)
        y1.append(i * i * -90 / (15 * 15) + 90)
        zero.append(0)
        i += 0.1

    # OLAP

    i = 0
    x3 = []
    y3 = []
    zero2 = []
    while i <= 99:
        x3.append(i)
        y3.append(1 / (0.00075 * (i - 100)) + 100)
        zero2.append(0)
        i += 1
    plot.ax.fill_between(x3, y3, zero2, color=Color.ORANGE, linewidth=0, alpha=0.2)
    plot.ax.fill_between(x1, y1, zero, color=Color.BLUE, linewidth=0, alpha=0.3)
    plot.ax.fill_between(y1, y2, zero, color=Color.RED, linewidth=0, alpha=0.3)

    # plot.plot(x=[16, 16], y=[55, 1000], color=Color.DARKGRAY, linestyle="--",linewidth=1)
    plot.plot(x=[16, 16], y=[0, 43], color=Color.DARKGRAY, linestyle="--", linewidth=1)
    plot.plot(x=[16, 16], y=[51, 55.5], color=Color.DARKGRAY, linestyle="--", linewidth=1)
    plot.plot(x=[16, 16], y=[64.5, 70], color=Color.DARKGRAY, linestyle="--", linewidth=1)
    plot.plot(x=[16, 16], y=[76, 1000], color=Color.DARKGRAY, linestyle="--", linewidth=1)

    plot.plot(x=[0, 48], y=[32, 32], color=Color.DARKGRAY, linestyle="--", linewidth=1)
    plot.plot(x=[52, 59], y=[32, 32], color=Color.DARKGRAY, linestyle="--", linewidth=1)
    plot.plot(x=[64, 70], y=[32, 32], color=Color.DARKGRAY, linestyle="--", linewidth=1)
    plot.plot(x=[75, 1000], y=[32, 32], color=Color.DARKGRAY, linestyle="--", linewidth=1)

    plot.plot(x=x1, y=y1, color=Color.BLUE)
    plot.plot(x=y1, y=y2, color=Color.RED)
    plot.plot(x=x3, y=y3, color=Color.ORANGE)

    plot.ax.text(1, 95, "\\textbf{OLAP}", fontsize=8, ha='left', va='center', color=Color.DARKGRAY)
    plot.ax.text(91, 10, "\\textbf{OLTP}", fontsize=8, ha='left', va='center', color=Color.DARKGRAY)
    plot.ax.text(50, 85, "\\textbf{HTAP}", fontsize=8, ha='left', va='center', color=Color.DARKGRAY)

    props = dict(facecolor=Color.BLUE, edgecolor=Color.BLUE, boxstyle='square,pad=0.2', alpha=0.5, lw=1)
    plot.ax.text(6, 85, "column store", fontsize=7, ha='left', va='top', color=Color.DARKGRAY, rotation=-30)
    plot.ax.text(9, 70, "compression", fontsize=7, ha='left', va='top', color=Color.DARKGRAY, rotation=-30)
    plot.ax.text(11, 55, "seq. scan", fontsize=7, ha='left', va='top', color=Color.DARKGRAY, rotation=-30)

    plot.ax.text(65, 18, "indexes", fontsize=7, ha='left', va='bottom', color=Color.DARKGRAY, rotation=45)
    plot.ax.text(55, 20, "row store", fontsize=7, ha='left', va='bottom', color=Color.DARKGRAY, rotation=45)
    plot.ax.text(45, 23, "point lookups", fontsize=7, ha='left', va='bottom', color=Color.DARKGRAY, rotation=45)

    frame.save(os.path.join("plots", "front", f"front"), format=Format.PDF)
    frame.save(os.path.join("plots", "front", f"front"), format=Format.PGF)
