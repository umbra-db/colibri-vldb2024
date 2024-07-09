import os

import numpy as np
import pandas as pd

from plot.barplot import BarPlot
from plot.color import Color
from plot.formatter import Decorator, Grid, LabelFormatter, Scale, ThreadsFormatter, ThroughputFormatter
from plot.frame import latexify, cm2inch, Format, GridFrame
from plot.lineplot import LinePlot
from plot.location import Location
from plot.marker import Marker
from plot.pattern import Pattern

systems = ["colibri", "umbra", "postgres", "dbms-x", "dbms-y", "singlestore", "duckdb"]
systems100 = ["colibri", "umbra", "postgres", "dbms-x", "dbms-y", "singlestore", "duckdb"]
systems1000 = ["colibri", "umbra", "postgres", "dbms-x", "dbms-y", "singlestore", "duckdb"]
cores = [0, 1, 2, 4, 8, 16, 32, 64]
cores_oltp = [1, 2, 4, 8, 16, 24, 32, 40, 48, 56, 64]

colors = [Color.ORANGE, Color.DARKBLUE, Color.GREEN, Color.LIGHTRED, Color.LIGHTBLUE, Color.LIGHTYELLOW, Color.RED]
labels = ["Colibri", "Umbra", "Postgres", "DBMS X", "DBMS Y", "SingleStore", "DuckDB"]
markers = [Marker.DIAMOND, Marker.CIRCLE, Marker.TRIANGLE_DOWN, Marker.TRIANGLE_UP, Marker.SQUARE, Marker.STAR, Marker.CROSS]


def load_tps(benchmark, scalefactor):
    throughput = {}
    for system in systems:
        throughput[system] = []

        for core in cores_oltp:
            filename = f"results/epyc/{system}/{benchmark}/{scalefactor}_{core}_0_client_trace.csv"
            if os.path.exists(filename):
                data = pd.read_csv(filename, header=None)
                throughput[system].append((data.iloc[1199, 1] - data.iloc[300, 1]) / 90)

    for system in systems:
        print("%s throughput: %s" % (system, throughput[system]))

    return throughput


def load_throughputs(benchmark, scalefactor, systems=systems, begin=30, end=120):
    duration = end - begin
    begin = begin * 10
    end = end * 10 - 1

    throughputs = {}
    for system in systems:
        throughputs[system] = [[], [], [], []]

        for x in cores:
            for y in cores:
                if x == 0 and y == 0:
                    continue

                filename = f"results/epyc/{system}/{benchmark}/{scalefactor}_{x}_{y}_client_trace.csv"
                if os.path.exists(filename):
                    data = pd.read_csv(filename, header=None)
                    try:
                        throughputs[system][0].append((data.iloc[end, 1] - data.iloc[begin, 1]) / duration)
                        throughputs[system][1].append((data.iloc[end, 2] - data.iloc[begin, 2]) / duration)
                        throughputs[system][2].append(x)
                        throughputs[system][3].append(y)
                    except Exception as e:
                        print(f"Error {filename}: {e}")

    return throughputs


def load_raw_data(benchmark, scalefactor, systems=systems, begin=30, end=120, mode=""):
    duration = end - begin
    begin = begin * 10
    end = end * 10 - 1

    frontier = {}
    for system in systems:
        throughputs = []

        for x in cores:
            for y in cores:
                if x == 0 and y == 0:
                    continue

                filename = f"results/epyc/{system}/{benchmark}/{scalefactor}_{x}_{y}{mode}_client_trace.csv"
                if os.path.exists(filename):
                    data = pd.read_csv(filename, header=None)
                    try:
                        throughputs.append(((data.iloc[end, 1] - data.iloc[begin, 1]) / duration, (data.iloc[end, 2] - data.iloc[begin, 2]) / duration, x, y))
                    except Exception as e:
                        print(f"Error {filename}: {e}")

        throughputs = sorted(throughputs)

        # Compute frontier
        frontier_idx = []
        drop = 0
        for i in range(0, len(throughputs)):
            for j in range(0, len(throughputs)):
                if ((throughputs[i][0] <= throughputs[j][0]) and (throughputs[i][1] < throughputs[j][1])) or ((throughputs[i][0] < throughputs[j][0]) and (throughputs[i][1] <= throughputs[j][1])):
                    drop = 1
            if drop == 0:
                frontier_idx.append(i)
            else:
                drop = 0

        frontier[system] = [[], [], [], []]
        for i in frontier_idx:
            frontier[system][0].append(throughputs[i][0])
            frontier[system][1].append(throughputs[i][1])
            frontier[system][2].append(throughputs[i][2])
            frontier[system][3].append(throughputs[i][3])

    for system in systems:
        print(f"{system} throughput: {list(zip(frontier[system][0], frontier[system][1], frontier[system][2], frontier[system][3]))}")

    return frontier


def load_single(benchmark, scalefactor, x, y, systems=systems, begin=30, end=120, mode=""):
    duration = end - begin
    begin = begin * 10
    end = end * 10 - 1

    result = []
    for system in systems:

        filename = f"results/epyc/{system}/{benchmark}/{scalefactor}_{x}_{y}{mode}_client_trace.csv"
        try:
            data = pd.read_csv(filename, header=None)
            result.append(((data.iloc[end, 1] - data.iloc[begin, 1]) / duration, (data.iloc[end, 2] - data.iloc[begin, 2]) / duration))
        except Exception as e:
            print(f"Error {filename}: {e}")

    return result


if __name__ == "__main__":
    def hattrick_tpch():
        print(f"\nHATTRICK100")
        latexify(hyperref=False, increase_font=-1)
        frame = GridFrame(width=cm2inch(17), height=cm2inch(3.25), n_columns=3, spacing=0.1)

        print("############################")
        plot = LinePlot(frame[0, 0], linewidth=1, markersize=2)
        plot.format_axes(yformatter=ThroughputFormatter(label="Analytical TP [Q/s]", ticks=[0, 5, 10, 15], min=0, max=18),
                         xformatter=ThroughputFormatter(label="Transactional TP [Tx/s]", ticks=[0, 50000, 100000, 150000, 200000], min=0, max=200000))

        frontier = load_raw_data("hattrick", 100, systems=systems100, begin=60, end=360)
        throughputs = load_throughputs("hattrick", 100, systems=systems100, begin=60, end=360)

        handles = []
        for i, system in reversed(list(enumerate(systems))):
            handles.append(plot.plot(x=frontier[system][0], y=np.array(frontier[system][1]), color=colors[i], marker=markers[i]))
            # plot.plot_scatter(x=np.array(throughputs[system][0]), y=np.array(throughputs[system][1]), color=colors[i], marker=markers[i])

        zoom = plot.zoom([0.5, 0.2, 0.48, 0.78], [0, 0, 0.01e6, 17])
        for i, system in reversed(list(enumerate(systems))):
            zoom.plot(x=frontier[system][0], y=np.array(frontier[system][1]), color=colors[i], marker=markers[i])
            zoom.plot_scatter(x=np.array(throughputs[system][0]), y=np.array(throughputs[system][1]), color=colors[i], marker=markers[i])

        frame.legend(handles=list(reversed(handles)), labels=labels, columns=7, bbox_to_anchor=(0.5, 1.125), location=Location.UPPER_CENTER)

        print("############################")
        plot = LinePlot(frame[0, 1], linewidth=1, markersize=2)
        plot.format_axes(yformatter=ThroughputFormatter(label="Analytical TP [Q/s]", ticks=[0, 2, 4, 6], min=0, max=6),
                         xformatter=ThroughputFormatter(label="Transactional TP [Tx/s]", ticks=[0, 0.5, 1, 1.5], min=0, max=1.5))

        frontier = load_raw_data("tpch", 100, systems=systems100, begin=60, end=6 * 60)
        throughputs = load_throughputs("tpch", 100, systems=systems100, begin=60, end=6 * 60)

        # for i, system in reversed(list(enumerate(systems100))):
        #    plot.plot_scatter(x=np.array(throughputs[system][0]), y=np.array(throughputs[system][1]), color=colors[i], marker=markers[i])
        for i, system in reversed(list(enumerate(systems100))):
            plot.plot(x=frontier[system][0], y=np.array(frontier[system][1]), color=colors[i], marker=markers[i])

        zoom = plot.zoom([0.05, 0.25, 0.5, 0.5], [0, 0, 0.6, 2])
        for i, system in reversed(list(enumerate(systems))):
            if system != "singlestore" and system != "umbra":
                continue
            zoom.plot_scatter(x=np.array(throughputs[system][0]), y=np.array(throughputs[system][1]), color=colors[i], marker=markers[i])
        for i, system in reversed(list(enumerate(systems))):
            zoom.plot(x=frontier[system][0], y=np.array(frontier[system][1]), color=colors[i], marker=markers[i])

        print("############################")
        plot = LinePlot(frame[0, 2], linewidth=1, markersize=2)
        plot.format_axes(yformatter=ThroughputFormatter(label="Analytical TP [Q/h]", ticks=[0, 200, 400, 600, 800], min=0, max=950),
                         xformatter=ThroughputFormatter(label="Transactional TP [Tx/h]", ticks=[0, 100, 200, 300, 400], min=0, max=450))

        frontier = load_raw_data("tpch", 1000, systems=systems1000, begin=0, end=30 * 60)
        throughputs = load_throughputs("tpch", 1000, systems=systems1000, begin=0, end=30 * 60)

        for i, system in enumerate(systems1000):
            plot.plot_scatter(x=np.array(throughputs[system][0]) * 60 * 60, y=np.array(throughputs[system][1]) * 60 * 60, color=colors[i], marker=markers[i])
        for i, system in enumerate(systems1000):
            plot.plot(x=np.array(frontier[system][0]) * 60 * 60, y=np.array(frontier[system][1]) * 60 * 60, color=colors[i], marker=markers[i])

        zoom = plot.zoom([0.5, 0.5, 0.48, 0.48], [0, 0, 20, 100])
        for i, system in reversed(list(enumerate(systems))):
            zoom.plot_scatter(x=np.array(throughputs[system][0]) * 60 * 60, y=np.array(throughputs[system][1]) * 60 * 60, color=colors[i], marker=markers[i])
            zoom.plot(x=np.array(frontier[system][0]) * 60 * 60, y=np.array(frontier[system][1]) * 60 * 60, color=colors[i], marker=markers[i])

        frame.save(os.path.join("plots", "hattrick_tpch", f"hattrick_tpch"), format=Format.PDF)
        frame.save(os.path.join("plots", "hattrick_tpch", f"hattrick_tpch"), format=Format.PGF)


    def tpcc_tpcch():
        print(f"\nTPCCH")
        latexify(hyperref=False, increase_font=-1)
        frame = GridFrame(width=cm2inch(11.5), height=cm2inch(3.25), n_columns=2, spacing=0.1)

        plot = LinePlot(frame[0, 0], linewidth=1, markersize=2)
        plot.format_axes(yformatter=ThroughputFormatter(ticks=[0e6, 0.1e6, 0.2e6, 0.3e6], label="Throughput [Tx/s]", min=0, max=0.35e6, grid=Grid.MINOR),
                         xformatter=ThreadsFormatter(ticks=[1, 4, 8, 16, 24, 32, 40, 48, 56, 64], label="Number of Clients", padding=0))

        throughput = load_tps("tpcc", 100)

        handles = []
        for i, system in enumerate(systems):
            print(i, system, throughput[system])
            handles.append(plot.plot(x=cores_oltp, y=np.array(throughput[system]), marker=markers[i], color=colors[i]))

        frame.legend(handles=handles, labels=labels, columns=6, bbox_to_anchor=(0.5, 1.125), location=Location.UPPER_CENTER)

        plot = LinePlot(frame[0, 1], linewidth=1, markersize=1.5)
        plot.format_axes(yformatter=ThroughputFormatter(label="Analytical TP [Q/s]", ticks=[0, 10, 20, 30], min=0, max=38),
                         xformatter=ThroughputFormatter(label="Transactional TP [Tx/s]", ticks=[0, 0.05e6, 0.1e6, 0.15e6, 0.2e6, 0.25e6, 0.3e6], min=0, max=0.325e6))

        frontier = load_raw_data("tpcch", 100)
        throughputs = load_throughputs("tpcch", 100)

        handles = []
        for i, system in reversed(list(enumerate(systems))):
            handles.append(plot.plot(x=frontier[system][0], y=np.array(frontier[system][1]), color=colors[i], marker=markers[i]))
        #    plot.plot_scatter(x=np.array(throughputs[system][0]), y=np.array(throughputs[system][1]), color=colors[i], marker=markers[i])

        zoom = plot.zoom([0.4, 0.4, 0.58, 0.58], [0, 0, 0.025e6, 5])
        for i, system in reversed(list(enumerate(systems))):
            zoom.plot(x=frontier[system][0], y=np.array(frontier[system][1]), color=colors[i], marker=markers[i])

        # frame.legend(handles=handles, labels=labels, columns=3, bbox_to_anchor=(0.5, 1.2), location=Location.UPPER_CENTER)
        frame.save(os.path.join("plots", "tpcc_tpcch", f"tpcc_tpcch"), format=Format.PDF)
        frame.save(os.path.join("plots", "tpcc_tpcch", f"tpcc_tpcch"), format=Format.PGF)


    def hybench():
        print(f"\nHYBENCH100")
        latexify(hyperref=False, increase_font=0)

        frame = GridFrame(width=cm2inch(8), height=cm2inch(2.75), n_columns=2)
        systems_hybench = ["colibri", "colibri_without_index", "umbra", "postgres"]
        colors_hybench = [Color.ORANGE, Color.LIGHTORANGE, Color.DARKBLUE, Color.GREEN]
        labels_hybench = ["Colibri", "Colibri (w/o indexes)", "Umbra", "Postgres"]

        handles = None
        for (i, x, y) in [(0, 64, 8), (1, 8, 64)]:
            plot = BarPlot(frame[0, i])
            plot.format_axes(yformatter=ThroughputFormatter(label="Throughput", min=1, max=450, scale=Scale.LOG, decorator=Decorator.AXISLABEL if i == 0 else Decorator.TICKS),
                             xformatter=LabelFormatter(ticklabels=["QPS", "TPS", "XPS"]))

            data = []
            for mode in ["_olap", "_oltp", "_olxp"]:
                d = [tps + qps for (tps, qps) in load_single("hybench", 100, x, y, systems=systems_hybench, begin=60, end=6 * 60, mode=mode)]
                data.append(d)
                print(d)

            handles = plot.plot(data, colors=colors_hybench, patterns=[Pattern.DENSE_BACK_DIAGONAL, Pattern.DENSE_DIAGONAL, Pattern.DENSE_CROSSED_DIAGONAL, Pattern.DENSE_SMALL_CIRCLES])
            plot.title(f"{x} TC, {y} AC", pad=0, fontsize=7)

        print(len(handles))
        print(len(labels_hybench))

        frame.legend(handles=handles, labels=labels_hybench, columns=4, bbox_to_anchor=(0.5, 1.15), location=Location.UPPER_CENTER)

        frame.save(os.path.join("plots", "hybench", f"hybench"), format=Format.PDF)
        frame.save(os.path.join("plots", "hybench", f"hybench"), format=Format.PGF)


    hattrick_tpch()
    tpcc_tpcch()
    hybench()
