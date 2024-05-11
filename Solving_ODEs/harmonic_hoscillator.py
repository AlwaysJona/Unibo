import numpy as np  
from matplotlib import pyplot as plt
from scipy.integrate import odeint
from matplotlib.animation import FuncAnimation

k = 3 # elastic constant


# S = (x, v)
def dSdt(S,t,k):
    x = S[0]
    v = S[1]
    return [v, -k*x]

x0 = 2
v0 = 0

# solution for continuous time
t = np.linspace(0, 100, 1000)
sol = odeint(dSdt, (x0,v0), t, args=(k,))
x = sol.T[0]
v = sol.T[1]

fig, ax = plt.subplots(2, figsize = (9,9))
fig.suptitle("Phase space of Harmonic Oscillator with Continuous (Top) and Descrete (Bottom) time")

line = ax[0].plot(x[0], v[0])[0]
ax[0].set_xlim(-5,5)
ax[0].set_ylim(-5,5)
ax[0].set_aspect('equal', adjustable='box')
ax[0].grid()
ax[0].set_xlabel("Position")
ax[0].set_ylabel("Velocity")

# solution for descrete time

t1 = np.linspace(0, 100, 100)
sol1 = odeint(dSdt, (x0,v0), t1, args=(k,))
x1 = sol1.T[0]
v1 = sol1.T[1]

line1 = ax[1].plot(x1[0], v1[0])[0]
ax[1].set_xlim(-5,5)
ax[1].set_ylim(-5,5)
ax[1].set_aspect('equal', adjustable='box')
ax[1].grid()
ax[1].set_xlabel("Position")
ax[1].set_ylabel("Velocity")

def Update(frame):
    line.set_xdata(x[:frame])
    line.set_ydata(v[:frame])

    line1.set_xdata(x1[:frame])
    line1.set_ydata(v1[:frame])
    
    return line, line1

anim = FuncAnimation(fig = fig, func=Update, frames=len(t), interval = 30)
plt.show()