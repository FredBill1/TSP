from subprocess import Popen
from os import listdir

TSP_EXE = "TSP_tsplib.exe"
DATA_SUFFIX = "_data.txt"
ANSWER_SUFFIX = "_answer.txt"
RESULT_FILE = "result.csv"
DIR = "test"
names = [s[: -len(DATA_SUFFIX)] for s in listdir(DIR) if s.endswith(DATA_SUFFIX)]
print(names)
print(f"Total: {len(names)} tests")

for i, name in enumerate(names):
    print(i, name)
    data_file = f"{DIR}/{name}{DATA_SUFFIX}"
    answer_file = f"{DIR}/{name}{ANSWER_SUFFIX}"
    Popen([TSP_EXE, data_file, answer_file, RESULT_FILE]).wait()
    print("\n")
