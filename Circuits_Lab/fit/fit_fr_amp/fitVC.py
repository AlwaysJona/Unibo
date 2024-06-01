# import libraries
import numpy as np
import matplotlib.pyplot as plt

from scipy.optimize import curve_fit

# ####################################################################
# initial parameters


# ####################################################################

R_gen = 50 # resistenza  
R_res = 100
RL = 120
V0 = 1 # ampiezza sorgente
L = 0.04658 # induttanza
C = 0.00000103 # capacità



# function to be fitted
def ampiezza_frequenza(f, R_gen, R_res, RL, V0, L, C): # y = 

    w = f*2*np.pi

    return (V0/(w*C))/(np.sqrt((R_gen + R_res + RL)**2 + (w*L - 1/(w*C))**2))




# ####################################################################

# get dataset from file0
filename = "frampVC.txt"
f, VC = np.loadtxt(filename, unpack=True)
stddev = 0.001805776
yerr = np.full(len(f),stddev)

# ####################################################################

P0 = [R_gen, R_res, RL,V0,L,C] 
popt, pcov = curve_fit(ampiezza_frequenza, # function to be fitted (defined above)
                       f, VC, # data
                       p0 = P0, # guessed parameters (used as starting values)
                       sigma = yerr, # error on y
                       maxfev=10000000 # the more difficult is the function, the longer it takes to fit the data. If maxfev is too short, it gives a RuntimeError
                       )

print( f"FIT PARAMS\n\n\nR_gen={popt[0]}\nR_res={popt[1]}\nRL={popt[2]}\nV0={popt[3]}\nL={popt[4]}\nC={popt[5]}\n\nCOVARIANCE MATRIX = \n{pcov}")

# ####################################################################
# plot data and fit
fig, ax = plt.subplots()
ax.errorbar(f,VC,yerr=yerr, fmt= '.', ecolor = "red") # uncomment to show also errorbars
ax.plot(f,VC, '.', label = "data", color = 'steelblue', markersize = 1)
ax.plot(f,ampiezza_frequenza(f, *popt), 
         color = 'orange', 
         label = "FIT")

ax.legend() 
ax.grid()
ax.set_xlabel("Frequenza (Hz)")
ax.set_ylabel("Ampiezza (V)")
ax.set_title("Fit VC in funzione della frequenza")
ax.set_ylim(0,1.5)

r_gen, r_res, rl, v0, l, c = popt # fitted parameters
res_freq = 1/(np.sqrt(l*c)*2*np.pi)

diag = pcov.diagonal(0)

dL = np.sqrt(diag[4])
dC = np.sqrt(diag[5])

uncertainty = (0.5*(c**(-0.5))*(l**(-1.5))*dL + 0.5*(c**(-1.5))*(l**(-0.5))*dC)/(2*np.pi)
print(f"Resonance Frequency = {res_freq} +- {uncertainty}")

u2 = np.sqrt(0.5*(1/((c**2)*(l**2)))*np.abs(pcov[4][5])+0.25*(1/(c*(l**3)))*pcov[4][4]+0.25*(1/((c**3)*l))*pcov[5][5])/(2*np.pi)
print(f"second uncert: {u2}")



plt.savefig("./fit_framp_VC.png") # --> Per salvare
plt.show() # --> Per visualizzare

