# import libraries
import numpy as np
import matplotlib.pyplot as plt

from scipy.optimize import curve_fit

# ####################################################################
# initial parameters


# ####################################################################

R = 100 + 120 + 50 # resistenza
Rgen = 50
R_res = 100
V0 = 1 # ampiezza sorgente
L = 0.04658 # induttanza
C = 0.00000103 # capacità
#C = 9.609431165054187e-07 # capacità, risultato fit

# function to be fitted
def ampiezza_frequenza(f,R_res, R, V0, L, C): # y = 

    w = f*2*np.pi

    return (V0*R_res)/(np.sqrt(R**2 + (w*L - 1/(w*C))**2))




# ####################################################################

# get dataset from file0
filename = "frampVR.txt"
f, VR = np.loadtxt(filename, unpack=True)
stddev = 0.001805776
yerr = np.full(len(f),stddev)

# ####################################################################

P0 = [R_res,R,V0,L,C] 
popt, pcov = curve_fit(ampiezza_frequenza, # function to be fitted (defined above)
                       f, VR, # data 
                       p0 = P0, # guessed parameters (used as starting values)
                       bounds=([R_res-0,R-0,V0-np.inf,L-np.inf,C-np.inf],[R_res+200,R+300,V0+2,L+np.inf,C+np.inf]),
                       sigma = yerr, # error on y
                       maxfev=1000000 # the more difficult is the function, the longer it takes to fit the data. If maxfev is too short, it gives a RuntimeError
                       )

print( f"FIT PARAMS\n\n\nR_res={popt[0]}\nR={popt[1]}\nV0={popt[2]}\nL={popt[3]}\nC={popt[4]}\n\nCOVARIANCE MATRIX = \n{pcov}")

# ####################################################################
# plot data and fit
fig, ax = plt.subplots()
ax.errorbar(f,VR,yerr=yerr, fmt= '.', ecolor = "red") # uncomment to show also errorbars
ax.plot(f,VR, '.', label = "data", color = 'steelblue', markersize = 1)
ax.plot(f,ampiezza_frequenza(f, *popt), 
         color = 'orange', 
         label = "FIT")

ax.legend() 
ax.grid()
ax.set_xlabel("Frequenza (Hz)")
ax.set_ylabel("Ampiezza (V)")
ax.set_title("Fit VR in funzione della frequenza")
ax.set_ylim(0,0.4)

r_r, r, v0, l, c = popt # fitted parameters
res_freq = 1/(np.sqrt(l*c)*2*np.pi)

diag = pcov.diagonal(0)

dL = np.sqrt(diag[3])
dC = np.sqrt(diag[4])

uncertainty = (0.5*(c**(-0.5))*(l**(-1.5))*dL + 0.5*(c**(-1.5))*(l**(-0.5))*dC)/(2*np.pi)
print(f"Resonance Frequency = {res_freq} +- {uncertainty}")

u2 = np.sqrt(0.5*(1/((c**2)*(l**2)))*np.abs(pcov[3][4])+0.25*(1/(c*(l**3)))*pcov[3][3]+0.25*(1/((c**3)*l))*pcov[4][4])/(2*np.pi)
print(f"second uncert: {u2}")

def rcs(x, y , yerr, popt): # funzione chi quadro ridotto
        residuals = y - ampiezza_frequenza(x, *popt)
        chi_squared = np.sum(((residuals/yerr)**2))
        ndof = len(y) - len(popt)
        return chi_squared / ndof

red = rcs(f,VR,yerr, popt)

print(red)

plt.savefig("./fit_framp_VR_reale.png") # --> Per salvare
plt.show() # --> Per visualizzare

