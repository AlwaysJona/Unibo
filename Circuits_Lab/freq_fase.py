from matplotlib import pyplot as plt
import numpy as np

filenameS = "./dati_in_txt/frfaseVS.txt"
filenameR = "./dati_in_txt/frfaseVR.txt"
filenameC = "./dati_in_txt/frfaseVC.txt"
filenameL = "./dati_in_txt/frfaseVL.txt"

frS, phS = np.loadtxt(filenameS, unpack=True)
frR, phR = np.loadtxt(filenameR, unpack=True)
#frC, phC = np.loadtxt(filenameC, unpack=True)
#frL, phL = np.loadtxt(filenameL, unpack=True)

plt.plot(frS,phS, label = "Sorgente", marker = ".")
plt.plot(frR,phR, label = "Resistenza", marker = ".")
#plt.plot(frC,phC, label = "Condensatore", marker = ".")
#plt.plot(frL,phL, label = "Induttanza", marker = ".")

plt.legend()
plt.grid()
plt.xlabel("Frequenza (Hz)")
plt.ylabel("Fase (RAD)")
plt.title("Andamento Fase in funzione della Frequenza")
plt.savefig("./grafico_ff_SR.png")
plt.show()
