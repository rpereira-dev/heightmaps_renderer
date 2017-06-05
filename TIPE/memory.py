import numpy as np
import matplotlib.pyplot as plt

Ko = 1 / 1024.0;
Mo = 1 / (1024.0 * 1024.0);
nmin = 4
nmax = 32 # nombre de point à calculer
SUB = 16.0 # nombre de points par subdivision de terrain

# nombre de points total sur la carte
def N(n):
    return (n * n)

# nombre de triangle total sur la carte
def T(n):
    return (2 * (n - 1) * (n - 1))

# nombre d'indice pour relié les triangles
def I(n):
    return (3 * T(n))
    
def M1(n):
    return (I(n) * 28)

def M2(n):
    return (N(n) * 28 + 2 * I(n))
    
def M3(n):
    return (N(n) * 16 + n / SUB * (16 * 4 + 2 * 4) + I(SUB))

x       = [n for n in range(nmin, nmax)]
yM1  = [M1(n) * Mo for n in x]
yM2  = [M2(n) * Mo for n in x]
yM3  = [M3(n) * Mo for n in x]

rM1 = [M1(n) / T(n) for n in x]
rM2  = [M2(n) / T(n) for n in x]
rM3  = [M3(n) / T(n) for n in x]

#plt.plot(x, yM1, label="M1(n)")
#plt.plot(x, yM2, label="M2(n)")
#plt.plot(x, yM3, label="M3(n)")

plt.plot(x, rM1, label="M1(n) / T(n)")
plt.plot(x, rM2, label="M2(n) / T(n)")
plt.plot(x, rM3, label="M3(n) / T(n)")

plt.legend()
plt.show()