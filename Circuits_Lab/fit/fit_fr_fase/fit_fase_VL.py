from matplotlib import pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

filename = "frfaseVL.txt"
f, phi = np.loadtxt(filename, unpack=True)

def fase_frequenza(f, R_res, R_gen, R_L, L, C):
    w = f*np.pi*2
    A = 1 - w**2*L*C
    B = w*(R_res + R_gen + R_L)*C 

    return np.arctan(A/B) + np.arctan(L*w/R_L)
R_res = 100  # resistenza  
R_gen = 50
R_L = 120
V0 = 1 # ampiezza sorgente
L = 0.04658 # induttanza
C = 0.00000103 # capacità
stddev = 0.006608131
yerr = np.full(len(f),stddev)

P0 = [R_res, R_gen, R_L, L, C]

popt, pcov = curve_fit(fase_frequenza,f,phi,P0,
                       sigma = yerr, 
                       bounds = ([R_res-0,R_gen-0,R_L-0,L-0,C-0],[R_res+150,R_gen+100,R_L+170,L+np.inf,C+np.inf]),
                       maxfev = 10000)
plt.plot(f, fase_frequenza(f,*popt), label = "Fit")
plt.plot(f,phi,".", label = "Dati")

print("\nCovariance Matrix")
print(pcov)

print(f"\nR_res = {popt[0]}\n\nR_gen = {popt[1]}\n\nR_L = {popt[2]}\n\nL = {popt[3]}\n\nC = {popt[4]}\n")


plt.xlabel("Frequenza (Hz)")
plt.ylabel("Fase (rad)")
plt.title("Andamento in frequenza della fase di VL")
plt.grid()
plt.legend()
plt.savefig("./fit_fase_VL.png")
plt.show()
