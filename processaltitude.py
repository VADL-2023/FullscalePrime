# -*- coding: utf-8 -*-
"""ProcessAltitude.ipynb

Automatically generated by Colaboratory.

Original file is located at
    https://colab.research.google.com/drive/1NX72Bc96jnHJsY5f_tcPD-VmDrZm8Jl2
"""

import matplotlib.pyplot as plt
import argparse

parser = argparse.ArgumentParser(description="Enter txt file")
parser.add_argument('-f','--file',required=True)
args = parser.parse_args()
file_path = args.file
file1 = open(file_path, 'r')
lines = file1.readlines()

times = [];
altitudes = [];
first = True;
for line in lines:
  line_split = line.split('\t ')
  if(first):
    first = False
  else:
    times.append(float(line_split[0]))
    altitudes.append(float(line_split[-1]))

firstIndex = len(altitudes) - altitudes[::-1].index(-99999.0)
times = times[firstIndex:]
initial_time = times[0]
times = [time - initial_time for time in times]
altitudes = altitudes[firstIndex:]

samples = list(range(0,len(altitudes)))

plt.plot(times,altitudes)
plt.xlabel("Time (ms)")
plt.ylabel("Height (m)")
plt.title("Height vs Time")
plt.savefig("HeightTime.png")

plt.plot(samples,altitudes)
plt.xlabel("Sample Number")
plt.ylabel("Height (m)")
plt.title("Height vs Sample Number")
plt.savefig("HeightSample.png")
