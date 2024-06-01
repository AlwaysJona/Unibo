# import libraries
import numpy as np
import matplotlib.pyplot as plt

from scipy.optimize import curve_fit

# ####################################################################
# initial parameters


# ####################################################################

R = 100 + 120 + 50 # resistenza  
V0 = 1 # ampiezza sorgente
w = 1200*(2*np.pi) # pulsazione
L = 0.04658 # induttanza
C = 0.00000103 # capacità


# function to be fitted
def ampiezza_capi_induttanza(t, R, V0, w, L, C): # y = 

    y = np.cos(w*t + (np.arctan((1-w**2*L*C)/R))+np.pi/2)

    return (w*L*V0)/(np.sqrt(R**2 + (w*L - 1/(w*C))**2))*y




# ####################################################################

# get dataset from file0
filename = "1200Hz_Adj.txt"
t, VS, tVR, VR, tVL, VL, tVC, VC = np.loadtxt(filename, unpack=True)
stddev = 0.005084553
yerr = np.full(len(t),stddev)

# ####################################################################

P0 = [R,V0,w,L,C] 
popt, pcov = curve_fit(ampiezza_capi_induttanza, # function to be fitted (defined above)
                       t, VL, # data 
                       p0 = P0, # guessed parameters (used as starting values)
                       sigma = yerr, # error on y
                       maxfev=1000000 # the more difficult is the function, the longer it takes to fit the data. If maxfev is too short, it gives a RuntimeError
                       )

print( f"FIT PARAMS\n\n\nR={popt[0]}\nV0={popt[1]}\nw={popt[2]}\nL={popt[3]}\nC={popt[4]}\n\nCOVARIANCE MATRIX = \n{pcov}")

# ####################################################################
# plot data and fit
fig, ax = plt.subplots()
ax.errorbar(t,VL,yerr=yerr, fmt= '.', ecolor = "red") # uncomment to show also errorbars
ax.plot(t,VL, '.', label = "data", color = 'steelblue', markersize = 1)
ax.plot(t,ampiezza_capi_induttanza(t, *popt), 
         color = 'orange', 
         label = "FIT")

ax.legend() 
ax.set_xlabel("Time (s)")
ax.set_ylabel("Signal (V)")
ax.set_title("Fit Potenziale ai capi dell'Induttanza")
ax.set_ylim(-1.5,1.5)
plt.savefig("./fitVL_Adj.png") # --> Per salvare
plt.show() # --> Per visualizzare

