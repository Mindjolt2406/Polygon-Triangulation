import numpy as np
import matplotlib.pyplot as plt

n = int(input())
lines = []
for i in range(n):
  l = [float(x) for x in input().split()]
  m = [(l[0],l[2]),(l[1], l[3])]
  lines.append(m)

data = []
for i in range(len(lines)):
    data.append(lines[i][0])
    data.append(lines[i][1])
    data.append('b')

plt.plot(*data)
plt.show()


