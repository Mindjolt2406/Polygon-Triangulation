import numpy as np
import matplotlib.pyplot as plt

def getInput(n,data):
  lines = []
  for i in range(n):
    l = [float(x) for x in input().split()]
    m = [(l[0],l[2]),(l[1], l[3])]
    lines.append(m)

  for i in range(len(lines)):
      data.append(lines[i][0])
      data.append(lines[i][1])
      data.append('b')

sidesNum = int(input())
data = []
getInput(sidesNum,data)

plt.plot(*data)
plt.show()

monotoneNum = int(input())
getInput(monotoneNum,data)
plt.plot(*data)
plt.show()

monotoneTriangleNum = int(input())
for i in range(monotoneTriangleNum):
  triangleNum = int(input())
  getInput(triangleNum,data)
  plt.plot(*data)
  plt.show()
