from subprocess import Popen, PIPE
from os.path import join, realpath, dirname
from typing import List, Tuple
from random import randint
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
from matplotlib.animation import FuncAnimation
import threading
import numpy as np

DIR = dirname(realpath(__file__))
TSP_EXE = join(DIR, "..", "build", "TSP")
fig, ax = plt.subplots()

N = 200
LIM = 10


(ax_verts,) = ax.plot([0], [0], marker="o", markersize=2, linestyle="", color="black")
ax_edges = Polygon([[0, 0], [0, 0]], fill=False, color="red")
ax.add_patch(ax_edges)
ax.set_xlim(0 - 0.5, LIM + 0.5)
ax.set_ylim(0 - 0.5, LIM + 0.5)

shutting_down = threading.Event()

verts = [[0, 0], [0, 0]]
edges = [[0, 0], [0, 0]]
flag = False
lock = threading.Lock()


def run_tsp():
    global verts, edges, flag
    while not shutting_down.is_set():
        cur_verts = np.random.uniform(0, LIM, (N, 2))
        Input = f"{N}\n" + "\n".join(["{} {}".format(x, y) for x, y in cur_verts])
        out, err = Popen([TSP_EXE], stdin=PIPE, stdout=PIPE).communicate(Input.encode("ascii"))
        res = [int(v) for v in out.decode("ascii").split()]
        cur_verts_t = cur_verts.transpose()
        with lock:
            verts = cur_verts_t
            edges = cur_verts[res]
            flag = True


def draw_frame(*_):
    global verts, edges, flag
    with lock:
        if flag:
            flag = False
            ax_verts.set_data(verts)
            ax_edges.set_xy(edges)
    return [ax_verts, ax_edges]


worker = threading.Thread(target=run_tsp)
worker.start()

animation = FuncAnimation(fig, draw_frame, init_func=draw_frame, interval=1, blit=True)
plt.show()

shutting_down.set()
worker.join()
