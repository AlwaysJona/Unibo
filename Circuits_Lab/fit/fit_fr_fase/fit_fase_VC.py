from matplotlib import pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

filename = "frfaseVC.txt"
f, phi = np.loadtxt(filename, unpack=True)

def fase_frequenza(f, R_res, R_gen, R_L, L, C):
    w = f*np.pi*2
    A = 1 - w**2*L*C
    B = w*(R_res + R_gen + R_L)*C

    return (np.arctan(A/B) - np.pi/2)

R_res = 100  # resistenza  
R_gen = 50
R_L = 120
V0 = 1 # ampiezza sorgente
#L = 0.04658 # induttanza
L = 0.05093243433106509 # induttanza
#C = 0.00000103 # capacità
C = 9.452295640612566e-07 # capacità
stddev = 0.038234562
yerr = np.full(len(f),stddev)

P0 = [R_res, R_gen, R_L, L, C]

popt, pcov = curve_fit(fase_frequenza,f,phi,P0,sigma = yerr,
                       bounds=([R_res-100, R_gen-100, R_L-100, L-np.inf, C-np.inf],[R_res+200, R_gen+200, R_L+200, L+0.5, C+1]),
                        maxfev = 10000)
plt.plot(f, fase_frequenza(f,*popt), label = "Fit")
plt.plot(f,phi,".", label = "Dati")

print("\nCovariance Matrix")
print(pcov)

print(f"\nR_res = {popt[0]}\n\nR_gen = {popt[1]}\n\nR_L = {popt[2]}\n\nL = {popt[3]}\n\nC = {popt[4]}\n")


plt.xlabel("Frequenza (Hz)")
plt.ylabel("Fase (rad)")
plt.title("Andamento in frequenza della fase di VC")
plt.grid()
plt.legend()
plt.savefig("./fit_fase_VC.png")
plt.show()
