from subprocess import Popen, PIPE
from os.path import join, realpath, dirname
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
import numpy as np

DIR = dirname(realpath(__file__))
InputFile = join(DIR, "input.txt")
TSP_EXE = join(DIR, "..", "build", "TSP")

fig, ax = plt.subplots()

N = 200
LIM = 10


(ax_verts,) = ax.plot([0], [0], marker="o", markersize=2, linestyle="", color="black")
ax_edges = Polygon([[0, 0], [0, 0]], fill=False, color="red")
ax.add_patch(ax_edges)
ax.set_xlim(0 - 0.5, LIM + 0.5)
ax.set_ylim(0 - 0.5, LIM + 0.5)


def run_tsp():
    verts = np.random.uniform(0, LIM, (N, 2))
    Input = f"{N}\n" + "\n".join(["{} {}".format(x, y) for x, y in verts])
    out, err = Popen([TSP_EXE], stdin=PIPE, stdout=PIPE).communicate(Input.encode("ascii"))
    res = [int(v) for v in out.decode("ascii").split()]
    ax_verts.set_data(*verts.transpose())
    ax_edges.set_xy(verts[res])
    plt.draw()
    return Input


plt.ion()
plt.show()

while True:
    Input = run_tsp()
    query = input("Press enter to continue,'q' to quit,'s' to save Input: ")
    if query == "q":
        break
    elif query == "s":
        with open(InputFile, "w") as f:
            f.write(Input)
