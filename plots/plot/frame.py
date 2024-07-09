import os
from enum import Enum
from typing import Tuple, Dict

import matplotlib.pyplot as plt
from matplotlib.artist import Artist
from matplotlib.axes import Axes

from plot.location import Location


def latexify(hyperref: bool = False, increase_font: int = 0, increase_legend_font: int = 0):
    """Set up matplotlib's RC params for LaTeX plotting.
    Call this before plotting a figure.
    Parameters
    ----------
    :param hyperref: bool, optional
    """

    # code adapted from http://www.scipy.org/Cookbook/Matplotlib/LaTeX_Examples

    packages = r'\usepackage[utf8]{inputenc} ' \
               r'\usepackage{xcolor} ' \
               r'\usepackage{amssymb} ' \
               r'\usepackage{ifsym} ' \
               r'\usepackage[T1]{fontenc} ' \
               r'\usepackage{libertinus} ' \
               r'\usepackage[scaled=0.85]{beramono} ' \
               r'\usepackage{graphicx}'

    if hyperref:
        packages += r'\usepackage{hyperref} '

    params = {
        'backend': 'ps',
        'pgf.rcfonts': False,
        'axes.labelsize': 9 + increase_font,  # fontsize for x and y labels
        'axes.titlesize': 10 + increase_font,
        'font.size': 9 + increase_font,
        'legend.fontsize': 8 + increase_font + increase_legend_font,
        'legend.handlelength': 1,
        'legend.handletextpad': 0.5,
        'legend.labelspacing': 0.1,  # was 0.1
        'legend.columnspacing': 1.5,
        'legend.borderpad': 0.3,
        'xtick.labelsize': 8 + increase_font,
        'ytick.labelsize': 8 + increase_font,
        'axes.labelpad': 3,
        'axes.titlepad': 3,
        'text.usetex': True,
        'font.family': 'serif',
        'text.latex.preamble': packages,
        'pgf.preamble': packages
    }

    plt.rcParams.update(params)


def cm2inch(value):
    return value / 2.54


class Format(str, Enum):
    PNG = "png"
    PDF = "pdf"
    PGF = "pgf"


class Frame:
    def __init__(self, ax):
        self.ax = ax

    def get_axes(self) -> Axes:
        return self.ax

    def clear(self):
        self.ax.remove()

    def title(self, text: str, horizontal_alignment: str = 'center', vertical_alignment: str = 'top', pad: float = 6.0, y: float = 1.0):
        plt.title(label=text, horizontalalignment=horizontal_alignment, verticalalignment=vertical_alignment, pad=pad, y=y)

    def legend(self, handles: [Artist], labels: [str], columns: int = 1, bbox_to_anchor: (float, float) = (1, 0.5), location: Location = Location.BEST, handlelength: float = None,
               handleheight: float = None):
        assert len(handles) == len(labels)
        plt.figlegend(handles, labels, ncol=columns, bbox_to_anchor=bbox_to_anchor, loc=location, frameon=False, framealpha=1, handlelength=handlelength, handleheight=handleheight)

    def save(self, path: str, format: Format = Format.PDF, padding: float = 0.0):
        dir = os.path.dirname(path)
        if not os.path.exists(dir) and dir != '':
            os.makedirs(dir)
        plt.savefig(path + "." + format, bbox_inches='tight', pad_inches=padding, dpi=300)


class SingleFrame(Frame):
    def __init__(self, width: float, height: float):
        self.fig = plt.figure(constrained_layout=True, figsize=(width, height))
        super().__init__(self.fig.add_subplot(111))


class GridFrame(Frame):
    def __init__(self, width: float, height: float, n_rows: int = 1, n_columns: int = 1, gridspec: Dict = None, sharex: bool = False, sharey: bool = False, spacing: float = None):
        super().__init__(None)

        self.fig, axs = plt.subplots(n_rows, n_columns, constrained_layout=True, figsize=(width, height), gridspec_kw=gridspec, sharex=sharex, sharey=sharey)
        if n_rows == 1 and n_columns == 1:
            self.axs = [[axs]]
        elif n_rows == 1:
            self.axs = [axs]
        elif n_columns == 1:
            self.axs = [[ax] for ax in axs]
        else:
            self.axs = axs
            
        self.fig.get_layout_engine().set(hspace=spacing, wspace=spacing)

    def get_axes(self):
        raise NotImplementedError

    def __getitem__(self, item: Tuple[int, int]) -> Frame:
        return Frame(self.axs[item[0]][item[1]])
