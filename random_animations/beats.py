import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

fig, ax = plt.subplots()


time = 10000
t = np.linspace(0, time, time)

v = 340 # wave speed
w = 2*np.pi / 10 # angular frequency
k = w/v # wave number

w1 = 2*np.pi / 11
k1 = w1/v

print(f"Wave length: {2*np.pi/k}")

x = np.linspace(0,100000,100000)

def progressive_wave(t,x,k,w):
    return np.sin(k*x - w*t)

line = ax.plot(x,progressive_wave(0,x,k,w))[0]
ax.set_ylim([-3,3])

def animation(frame):
    line.set_data(x,progressive_wave(frame,x,k,w)+progressive_wave(frame,x,k1,w1))
    return line

anim = FuncAnimation(fig,animation,time, interval = 20)

ax.set_xlabel("x")
ax.set_ylabel("y")
ax.set_title("Beats")

plt.show()
