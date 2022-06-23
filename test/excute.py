from subprocess import Popen, PIPE
from os.path import join, realpath, dirname
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
import numpy as np
from time import time

DIR = dirname(realpath(__file__))
InputFile = join(DIR, "input.txt")
TSP_EXE = join(DIR, "..", "build", "Release", "TSP")

fig, ax = plt.subplots()

N = 200
LIM = 10

mode = int(input("Mode <0/1>: "))

(ax_verts,) = ax.plot([0], [0], marker="o", markersize=2, linestyle="", color="black")
ax_edges = Polygon([[0, 0], [0, 0]], closed=mode == 0, fill=False, color="red")
ax.add_patch(ax_edges)
length_text = ax.text(-0.4, -0.4, "0")
ax.set_aspect("equal", adjustable="datalim")


def run_tsp():
    verts = np.random.uniform(0, LIM, (N, 2))
    Input = f"{mode}\n{N}\n" + "\n".join(["{} {}".format(x, y) for x, y in verts])
    start = time()
    out, err = Popen([TSP_EXE], stdin=PIPE, stdout=PIPE).communicate(Input.encode("ascii"))
    print(f"Time: {time() - start}")
    out = out.decode("ascii").split()
    length = float(out[0])
    length_text.set_text(f"{length:.2f}")
    res = [int(v) for v in out[1:]]
    ax_verts.set_data(*verts.transpose())
    ax_edges.set_xy(verts[res])
    ax.relim()
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
