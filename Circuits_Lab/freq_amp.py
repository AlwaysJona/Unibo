from matplotlib import pyplot as plt
import numpy as np

filenameS = "./dati_in_txt/frampVS.txt"
filenameR = "./dati_in_txt/frampVR.txt"
filenameC = "./dati_in_txt/frampVC.txt"
filenameL = "./dati_in_txt/frampVL.txt"

frS, amS = np.loadtxt(filenameS, unpack=True)
frR, amR = np.loadtxt(filenameR, unpack=True)
frC, amC = np.loadtxt(filenameC, unpack=True)
frL, amL = np.loadtxt(filenameL, unpack=True)

plt.plot(frS,amS, label = "Sorgente", marker = ".")
plt.plot(frR,amR, label = "Resistenza", marker = ".")
plt.plot(frC,amC, label = "Condensatore", marker = ".")
plt.plot(frL,amL, label = "Induttanza", marker = ".")

plt.legend()
plt.grid()
plt.xlabel("Frequenza (Hz)")
plt.ylabel("Ampiezza (V)")
plt.title("Andamento Ampiezza in funzione della Frequenza")
plt.savefig("./grafico_freq_amp.png")
plt.show()
