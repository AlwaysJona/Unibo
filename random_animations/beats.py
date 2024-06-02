import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

fig, ax = plt.subplots()


time = 10000
N = time*500
t = np.linspace(0, time, N)

v = 340 # wave speed
w = 2*np.pi * 10 # angular frequency
k = w/v # wave number

w1 = 2*np.pi * 11
k1 = w1/v

print(f"Wave length: {2*np.pi/k}\nPeriod: {2*np.pi/w}")

x = np.linspace(0,500,1000)

def progressive_wave(t,x,k,w):
    return np.sin(k*x - w*t)

line = ax.plot(x,progressive_wave(0,x,k,w))[0]
ax.set_ylim([-3,3])

def animation(frame):
    line.set_data(x,progressive_wave(t[frame],x,k,w)+progressive_wave(t[frame],x,k1,w1))
    return line

anim = FuncAnimation(fig, animation, N, interval = 20)

ax.set_xlabel("x")
ax.set_ylabel("y")
ax.set_title("Beats")

plt.show()
