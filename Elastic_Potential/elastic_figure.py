import elastic_equations as eq
from matplotlib import pyplot as plt
import matplotlib.animation as animation
import numpy as np

# initial conditions
k = (2*np.pi/4000)**2 # elastic constant
x0 = 2 # initial position
v0 = 0 # initial velocity
t0 = 0 # time of initial conditions
dt = 30 # time interval


t = np.arange(0, dt*500, dt)
x = eq.Position(k,x0,v0,t0,t)

fig, ax = plt.subplots(2, figsize = (7,7))

ax[0].plot(t,x) # Time-Position Plot

p0 = plt.Circle((0,x0), 0.3, color = 'r')
ax[0].add_patch(p0)

ax[1].set_aspect('equal', adjustable='box') # x-y Plot, adjusting aspects of axes

p1 = plt.Circle((x0, 0), 0.2, color = 'r') # create moving p1
ax[1].add_patch(p1)
ax[1].set_xlim(-3,3)
ax[1].set_ylim(-3,3)

def Update(frame):
    p1.center = (x[frame], 0)
    p0.center = (t[frame], x[frame])
    return p0, p1

ani = animation.FuncAnimation(fig = fig,func=Update, frames=t.size, interval = dt)

plt.show()

