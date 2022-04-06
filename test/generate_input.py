import numpy as np
from os.path import join, realpath, dirname

DIR = dirname(realpath(__file__))
InputFile = join(DIR, "input.txt")

N = 200
LIM = 10
verts = np.random.uniform(0, LIM, (N, 2))

with open(InputFile, "w") as f:
    f.write(f"{N}\n")
    f.write("\n".join(f"{v[0]} {v[1]}" for v in verts))
