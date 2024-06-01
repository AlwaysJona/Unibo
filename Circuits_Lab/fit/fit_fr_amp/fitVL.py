# import libraries
import numpy as np
import matplotlib.pyplot as plt

from scipy.optimize import curve_fit

# ####################################################################
# initial parameters


# ####################################################################

R = 100 + 120 + 50 # resistenza  
V0 = 1 # ampiezza sorgente
L = 0.04658 # induttanza
C = 0.00000103 # capacità


# function to be fitted
def ampiezza_frequenza(f, R, V0, L, C): # y = 

    w = f*2*np.pi

    return (w*L*V0)/(np.sqrt(R**2 + (w*L - 1/(w*C))**2))




# ####################################################################

# get dataset from file0
filename = "frampVL.txt"
f, VL = np.loadtxt(filename, unpack=True)
stddev = 0.001805776
yerr = np.full(len(f),stddev)

# ####################################################################

P0 = [R,V0,L,C] 
popt, pcov = curve_fit(ampiezza_frequenza, # function to be fitted (defined above)
                       f, VL, # data 
                       p0 = P0, # guessed parameters (used as starting values)
                       sigma = yerr, # error on y
                       maxfev=1000000 # the more difficult is the function, the longer it takes to fit the data. If maxfev is too short, it gives a RuntimeError
                       )

print( f"FIT PARAMS\n\n\nR={popt[0]}\nV0={popt[1]}\nL={popt[2]}\nC={popt[3]}\n\nCOVARIANCE MATRIX = \n{pcov}")

# ####################################################################
# plot data and fit
fig, ax = plt.subplots()
ax.errorbar(f,VL,yerr=yerr, fmt= '.', ecolor = "red") # uncomment to show also errorbars
ax.plot(f,VL, '.', label = "data", color = 'steelblue', markersize = 1)
ax.plot(f,ampiezza_frequenza(f, *popt), 
         color = 'orange', 
         label = "FIT")

ax.legend() 
ax.grid()
ax.set_xlabel("Frequency (Hz)")
ax.set_ylabel("Signal (V)")
ax.set_title("Fit VL in funzione della frequenza")
ax.set_ylim(0,1.5)
plt.savefig("./fit_framp_VL.png") # --> Per salvare
plt.show() # --> Per visualizzare

