import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

filename = "phase.txt"

f, c2, c3, c4 = np.loadtxt(filename, unpack=True)

plt.plot(f,c2,".", label = "C2", markersize = 8)
plt.plot(f,c3,".", label = "C3", markersize = 8)
plt.plot(f,c4,".", label = "C4", markersize = 8)

def line(x, m, q):
    y = m*x + q
    return y

P2 = [6.66585E-05,0.000771726]
P3 = [0.00013783,-0.000545446]
P4 = [0.000205822,-0.001541182]

yerr = np.full(len(f), 0.00001)

popt2, pcov2 = curve_fit(line, f,c2,P2, sigma=yerr)
popt3, pcov3 = curve_fit(line, f,c3,P3, sigma=yerr)
popt4, pcov4 = curve_fit(line, f,c4,P4, sigma=yerr)

plt.plot(f,line(f,*popt2),label = "Fit C2")
plt.plot(f,line(f,*popt3),label = "Fit C3")
plt.plot(f,line(f,*popt4),label = "Fit C4")

plt.title("Sfasamento in funzione della frequenza")
plt.xlabel("Frequenza (Hz)")
plt.ylabel("Sfasamento (Rad)")
plt.legend()
plt.grid()
plt.savefig("./phase.png")
plt.show()