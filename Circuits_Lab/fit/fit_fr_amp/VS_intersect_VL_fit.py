# import libraries
import numpy as np
import matplotlib.pyplot as plt

from scipy.optimize import curve_fit

filenameS = "frampVS.txt"
fS, VS = np.loadtxt(filenameS, unpack=True)

filenameL = "frampVL.txt"
fL, VL = np.loadtxt(filenameL, unpack=True)


#############################################################

def VS_formula(fS, R, V0, L, C):
    Rgen = 50 # impedenza generatore

    # function to be fitted
    def ampiezza_frequenza(f, R, V0, L, C): # y = 

        w = f*2*np.pi

        A = Rgen**2/(R**2 + (w*L-1/(w*C))**2)

        B = 2*Rgen*np.cos(np.arctan((1-w**2*L*C)/(R*w*C)))/np.sqrt(R**2 + (w*L-1/(w*C))**2)

        return V0*np.sqrt(1 + A - B)
    
    return ampiezza_frequenza(fS, R, V0, L, C)

#############################################################

def VL_formula(fL, R, V0, L, C):
    RL = 120
    def ampiezza_frequenza(f, R, V0, L, C): # y = 

        w = f*2*np.pi

        A = RL**2 + L**2*w**2
        B = R**2 + (w*L-1/w/C)**2

        return V0*np.sqrt(A/B)

    return ampiezza_frequenza(fL, R, V0, L, C)

#############################################################

def VS_analysis():
    R = 100 + 120 + 50 # resistenza  
    V0 = 1 # ampiezza sorgente
    L = 0.04658 # induttanza
    C = 0.00000103 # capacità

    stddev = 0.001805776
    yerr = np.full(len(fS),stddev)

    P0 = [R,V0,L,C] 
    popt, pcov = curve_fit(VS_formula, # function to be fitted (defined above)
                           fS, VS, # data 
                           p0 = P0, # guessed parameters (used as starting values)
                           sigma = yerr, # error on y
                           maxfev=1000000 # the more difficult is the function, the longer it takes to fit the data. If maxfev is too short, it gives a RuntimeError
                           )
    
    return popt, pcov

#############################################################

def VL_analysis():
    R = 100 + 120 + 50 # resistenza  
    V0 = 1 # ampiezza sorgente
    L = 0.04658 # induttanza
    C = 0.00000103 # capacità
   
    stddev = 0.001805776
    yerr = np.full(len(fL),stddev)

    P0 = [R,V0,L,C] 
    popt, pcov = curve_fit(VL_formula, # function to be fitted (defined above)
                           fL, VL, # data 
                           p0 = P0, # guessed parameters (used as starting values)
                           sigma = yerr, # error on y
                           maxfev=1000000 # the more difficult is the function, the longer it takes to fit the data. If maxfev is too short, it gives a RuntimeError
                           )

    return popt, pcov

#############################################################

poptS, pcovS = VS_analysis()
poptL, pcovL = VL_analysis()

plt.plot(fS,VS_formula(fS,*poptS),label = "Fit Sorgente")
plt.plot(fS,VS, ".", label = "Sorgente", markersize = 4)
plt.plot(fL,VL_formula(fL,*poptL), label = "Fit Induttanza")
plt.plot(fL,VL, ".", label = "Induttanza")

plt.xlabel("Frequenza (Hz)")
plt.ylabel("Ampiezza (V)")

plt.title("Andamento in frequenza di VS e VL")
plt.legend()
plt.grid()

freq = np.linspace(500,900, 1000)
diff = 1
resonance = 500
for f in freq:
    d = np.abs(VS_formula(f,*poptS)-VL_formula(f,*poptL)) 
    if d <= diff:
        diff = d
        resonance = f

print(f"Resonance frequency: {resonance} Hz")
plt.show()
