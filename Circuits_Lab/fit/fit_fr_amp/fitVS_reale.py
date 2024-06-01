# import libraries
import numpy as np
import matplotlib.pyplot as plt

from scipy.optimize import curve_fit

# ####################################################################
# initial parameters


# ####################################################################

R = 100 + 120 + 50 # resistenza  
Rgen = 50 # impedenza generatore
V0 = 1 # ampiezza sorgente
L = 0.04658 # induttanza
C = 0.00000103 # capacità


# function to be fitted
def ampiezza_frequenza(f, R, V0, L, C): # y = 

    w = f*2*np.pi

    A = Rgen**2/(R**2 + (w*L-1/(w*C))**2)

    B = 2*Rgen*np.cos(np.arctan((1-w**2*L*C)/(R*w*C)))/np.sqrt(R**2 + (w*L-1/(w*C))**2)

    return V0*np.sqrt(1 + A - B)

def rcs(f, y, yerr, popt): # funzione chi quadro ridotto
        residuals = y - ampiezza_frequenza(f, *popt)
        chi_squared = np.sum(((residuals/yerr)**2))
        ndof = len(f) - len(popt)
        return chi_squared / ndof

# ####################################################################

# get dataset from file
filename = "frampVS.txt"
f, VS = np.loadtxt(filename, unpack=True)
stddev = 0.001805776
yerr = np.full(len(f),stddev)

# ####################################################################

P0 = [R,V0,L,C] 
popt, pcov = curve_fit(ampiezza_frequenza, # function to be fitted (defined above)
                       f, VS, # data 
                       p0 = P0, # guessed parameters (used as starting values)
                       sigma = yerr, # error on y
                       maxfev=1000000 # the more difficult is the function, the longer it takes to fit the data. If maxfev is too short, it gives a RuntimeError
                       )

print( f"FIT PARAMS\n\n\nR={popt[0]}\nV0={popt[1]}\nL={popt[2]}\nC={popt[3]}\n\nCOVARIANCE MATRIX = \n{pcov}")

# ####################################################################
# plot data and fit
fig, ax = plt.subplots()
ax.errorbar(f,VS,yerr=yerr, fmt= '.', ecolor = "red") # uncomment to show also errorbars
ax.plot(f,VS, '.', label = "data", color = 'steelblue', markersize = 0.01)
ax.plot(f,ampiezza_frequenza(f, *popt), 
         color = 'orange', 
         label = "FIT")

ax.legend() 
ax.grid()
ax.set_xlabel("Frequenza (Hz)")
ax.set_ylabel("Ampiezza (V)")
ax.set_title("Fit VS in funzione della frequenza")
ax.set_ylim(0.6,1.2)

max_f = 700
min_amp = 1
freq = np.linspace(0.1,1000,10000)
for f in freq:
    amp = ampiezza_frequenza(f,*popt)
    if amp < min_amp:
        min_amp = amp
        max_f = f
print(f"Min Frequency = {max_f}\nMin Amplitude = {min_amp}")

r, v0, l, c = popt # fitted parameters
res_freq = 1/(np.sqrt(l*c)*2*np.pi)

diag = pcov.diagonal(0)

dL = np.sqrt(diag[2])
dC = np.sqrt(diag[3])

uncertainty = (0.5*(c**(-0.5))*(l**(-1.5))*dL + 0.5*(c**(-1.5))*(l**(-0.5))*dC)/(2*np.pi)
print(f"Resonance Frequency = {res_freq} +- {uncertainty}")

u2 = np.sqrt(0.5*(1/((c**2)*(l**2)))*np.abs(pcov[2][3])+0.25*(1/(c*(l**3)))*pcov[2][2]+0.25*(1/((c**3)*l))*pcov[3][3])/(2*np.pi)
print(f"second uncert: {u2}")

Q = (1/r)*np.sqrt(l/c)
print(f"Quality Factor: {Q}")

plt.savefig("./fit_framp_VS_zoomed.png") # --> Per salvare
plt.show() # --> Per visualizzare

