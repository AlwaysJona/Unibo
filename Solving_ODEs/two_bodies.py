import numpy as np
from scipy.integrate import odeint
from matplotlib import pyplot as plt
from matplotlib.animation import FuncAnimation

m1 = 1000000000000
m2 = 1000000000000
G = 6.67e-11

# S = (x1, y1, x2, y2, vx1, vy1, vx2, vy2)

def dSdt(S, t, m1, m2, G):
    x1 = S[0]
    y1 = S[1]
    x2 = S[2]
    y2 = S[3]
    vx1 = S[4]
    vy1 = S[5]
    vx2 = S[6]
    vy2 = S[7]

    r = np.linalg.norm((x1-x2,y1-y2))

    return [vx1, vy1, vx2, vy2,
            G*m2*(x2-x1)/r**3, # ax1
            G*m2*(y2-y1)/r**3, # ay1
            G*m1*(x1-x2)/r**3, # ax2
            G*m1*(y1-y2)/r**3, # ay2
            ]

x10 = -1
y10 = -1
x20 = 2
y20 = 0

vx10 = 1
vy10 = 1
vx20 = 0
vy20 = -1

t = np.linspace(0, 1000, 100000)

sol = odeint(dSdt, (x10, y10, x20, y20, vx10, vy10, vx20, vy20), t, args=(m1,m2,G))

print(sol)

x1 = sol.T[0]
y1 = sol.T[1]
x2 = sol.T[2]
y2 = sol.T[3]

fig, ax = plt.subplots()
fig.suptitle("2 Body problem")
ax.set_xlim(-3,7)
ax.set_ylim(-3,3)

line1 = ax.plot(x1[0],y1[0])[0]
line2 = ax.plot(x2[0],y2[0])[0]

def Update(frame):
    line1.set_xdata(x1[:frame])
    line1.set_ydata(y1[:frame])
    line2.set_xdata(x2[:frame])
    line2.set_ydata(y2[:frame])
    return line1, line2

anim = FuncAnimation(fig=fig, func=Update, frames=len(t), interval = 30)
plt.show()

