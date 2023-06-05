#!/usr/bin/python3

import subprocess

with open("benchmark1.csv", "w") as resfile:
    for i in range(32, 4096):
        p = subprocess.run(["./benchmark1", f"{i}", f"{int(16000000/i)}"], stdout=resfile)
        p = subprocess.run(["./benchmark1", f"{i}", f"{int(16000000/i)}"], stdout=resfile)
        p = subprocess.run(["./benchmark1", f"{i}", f"{int(16000000/i)}"], stdout=resfile)
        p = subprocess.run(["./benchmark1", f"{i}", f"{int(16000000/i)}"], stdout=resfile)
        p = subprocess.run(["./benchmark1", f"{i}", f"{int(16000000/i)}"], stdout=resfile)
