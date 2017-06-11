import numpy as np
import matplotlib.pyplot as plt
import math

n = 1000
a = -1
b =  1
h = (b - a) / float(n)

x   = [a + i * h for i in range(0, n)]
dy  = [math.acos(xi) - ((-0.69813170079773212 * xi * xi - 0.87266462599716477) * xi + 1.5707963267948966) for xi in x]
y1  = [math.acos(xi) for xi in x]
y2  = [(-0.69813170079773212 * xi * xi - 0.87266462599716477) * xi + 1.5707963267948966 for xi in x]

plt.plot(x, dy, label="dy")
plt.plot(x, y1, label="acos")
plt.plot(x, y2, label="acos Lagrange")

plt.show()