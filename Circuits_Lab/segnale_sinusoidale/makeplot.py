import numpy as np
from matplotlib import pyplot as plt

filename = "1200Hz.txt"
TS, VS, TR, VR, TL, VL, TC, VC = np.loadtxt(filename, unpack=True)

plt.plot(TS, VS, label = "Sorgente", marker = ".")
plt.plot(TS, VR, label = "Resistenza", marker = ".")
plt.plot(TS, VC, label = "Condensatore", marker = ".")
plt.plot(TS, VL, label= "Induttanza", marker = ".")

plt.legend()
plt.xlim([0,0.00295])
plt.title("Tensioni a 1200Hz")
plt.xlabel("Tempo (s)")
plt.ylabel("Tensione (V)")
plt.grid()

plt.savefig("./Grafico1200Hz.png")
plt.show()