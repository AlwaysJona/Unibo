from matplotlib import pyplot as plt
import numpy as np

filenameS = "./frfaseVS.txt"
filenameR = "./frfaseVR.txt"
filenameC = "./frfaseVC.txt"
filenameL = "./frfaseVL.txt"

frS, phS = np.loadtxt(filenameS, unpack=True)
frR, phR = np.loadtxt(filenameR, unpack=True)
frC, phC = np.loadtxt(filenameC, unpack=True)
frL, phL = np.loadtxt(filenameL, unpack=True)

plt.plot(frS,phS, ".", label = "Sorgente")
plt.plot(frR,phR, ".", label = "Resistenza")
plt.plot(frC,phC, ".", label = "Condensatore")
plt.plot(frL,phL, ".", label = "Induttanza")

plt.legend()
plt.grid()
plt.xlabel("Frequenza (Hz)")
plt.ylabel("Fase (RAD)")
plt.title("Andamento Fase in funzione della Frequenza")
plt.savefig("./grafico_freq_fase.png")
plt.show()
