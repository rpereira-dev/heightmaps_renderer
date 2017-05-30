import numpy as np
import matplotlib.pyplot as plt

nmax = 4096

def N(n):
    return (n * n)

def T(n):
    return (2 * (n - 1) * (n - 1))

def I(n):
    return (3 * T(n))
    
def M1(n, S):
    return (I(n) * S)

def M2(n, S):
    return (N(n) * S + 2 * I(n))
    
def M3(n):
    return (M2(n, 18) + n / 16 * (16 + 2) * 4)

Ko = 1 / 1024.0;
Mo = 1 / (1024.0 * 1024.0);
    
x       = [n for n in range(2, nmax)]
yM1_28  = [M1(n, 28) * Mo for n in x]
yM2_28  = [M2(n, 28) * Mo for n in x]
yM2_18  = [M3(n) * Mo for n in x]
Tn      = [T(n) for n in x]

rM1_28  = [M1(n, 28) / T(n) for n in x]
rM2_28  = [M2(n, 28) / T(n) for n in x]
rM2_18  = [M3(n) / T(n) for n in x]

plt.plot(x, yM1_28, label="M1(n, 28)")
plt.plot(x, yM2_28, label="M2(n, 28)")
plt.plot(x, yM2_18, label="M2(n, 18)")

#plt.plot(x, rM1_28, label="M1(n, 28) / T(n)")
#plt.plot(x, rM2_28, label="M2(n, 28) / T(n)")
#plt.plot(x, rM2_18, label="M3(n, 18) / T(n)")

plt.legend()
plt.show()