import numpy as np
from matplotlib import pyplot as plt
import matplotlib.animation as animation
import orbit_funcs as orb


#constants
p = 3
e = 0.6

# array of angles
phi = np.linspace(0,2*2*np.pi,200)

# data for plot
x,y = orb.position(phi,p,e)

# create figure
fig, ax = plt.subplots()
# set axis limits
ax.set(xlim=[orb.position(np.pi,p,e)[0]-1,orb.position(0,p,e)[0]+1], 
       ylim=[orb.position(1.5*np.pi,p,e)[1]-1, orb.position(0.5*np.pi,p,e)[1]+1])
# plot first point
line = ax.plot(x[0],y[0], color = 'green')[0]

# lets go in space
sun = plt.Circle((0,0), 0.4, color = 'yellow')
fig.set_facecolor('black')
ax.add_patch(sun)
earth = plt.Circle((x[0],y[0]), 0.1, color = 'blue')
ax.add_patch(earth)


def init():
    earth.center = (0,0)
    ax.add_patch(earth)
    return earth,


# define function for animation
def update(frame):
    line.set_xdata(x[frame-10:frame])
    line.set_ydata(y[frame-10:frame])
    earth.center = (x[frame], y[frame])
    return line, earth


ani = animation.FuncAnimation(fig=fig,init_func = init,func=update,frames=phi.size, interval = 30, blit = True )
plt.axis('off')
plt.show()
    