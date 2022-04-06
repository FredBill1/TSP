from os.path import join, realpath, dirname
from subprocess import Popen, PIPE
import fileinput
from typing import List, Tuple
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
import numpy as np

DIR = dirname(realpath(__file__))
InputFile = join(DIR, "input.txt")
TSP_EXE = join(DIR, "..", "build", "TSP")


def get_input() -> Tuple[int, List[list]]:
    file = fileinput.input(InputFile)
    input = lambda: next(file).rstrip()
    N = int(input())
    res = np.array([[float(v) for v in input().split()] for _ in range(N)])
    file.close()
    return N, res


N, verts = get_input()
with open(InputFile, "r") as f:
    Input = f.read()
out, err = Popen([TSP_EXE], stdin=PIPE, stdout=PIPE).communicate(Input.encode("ascii"))
out = out.decode("ascii")
print(out)
res = [int(v) for v in out.split()]


fig, ax = plt.subplots()
ax.plot(*verts.transpose(), marker="o", markersize=2, linestyle="", color="black")
ax.add_patch(Polygon(verts[res], fill=False, color="red"))

plt.show()
