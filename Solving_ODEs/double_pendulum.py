import numpy as np
import sympy as smp
from scipy.integrate import odeint
import matplotlib.pyplot as plt
from matplotlib import animation
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import PillowWriter

# define symbols for parameters

L1, L2 = smp.symbols('L1 L2')
t, g = smp.symbols('t g')
m1, m2 = smp.symbols('m1 m2')

# define symbols for functions, the 2 angles are the degrees of freedom of the system 

the1, the2 = smp.symbols('theta_1, theta_2', cls=smp.Function)
the1 = the1(t)
the2 = the2(t)

# define the derivatives of the angles

the1_d = smp.diff(the1, t)
the2_d = smp.diff(the2, t)

the1_dd = smp.diff(the1_d, t)
the2_dd = smp.diff(the2_d, t)

# define Cartesian coordinates, to later compute the Kinetic Energy

x1 = L1*smp.sin(the1)
y1 = -L1*smp.cos(the1)
x2 = L1*smp.sin(the1) + L2*smp.sin(the2)
y2 = -L1*smp.cos(the1) - L2*smp.cos(the2)

# define Kinetic Energy

T1 = (1/2)*m1*((smp.diff(x1,t))**2+smp.diff(y1,t)**2)
T2 = (1/2)*m2*((smp.diff(x2,t))**2+smp.diff(y2,t)**2)
T = T1 + T2

# define Potential Energy

U1 = m1*g*y1
U2 = m2*g*y2
U = U1 + U2

# define Lagrangian

L = T - U

# define Euler-Lagrange Equations

EL1 = smp.diff(L, the1) - smp.diff(smp.diff(L,the1_d), t).simplify()
EL2 = smp.diff(L, the2) - smp.diff(smp.diff(L,the2_d), t).simplify()

# solve Euler-Lagrange Equations for the 2 second derivatives of the 2 angles

sols = smp.solve([EL1, EL2], (the1_dd, the2_dd), simplify = False, rational = False)

# now we have 2 second order ODEs, but we can turn them into 4 first order ODEs

dz1dt_f = smp.lambdify((t,g,m1,m2,L1,L2,the1,the2,the1_d,the2_d), sols[the1_dd]) # dz1dt = the1_dd
dz2dt_f = smp.lambdify((t,g,m1,m2,L1,L2,the1,the2,the1_d,the2_d), sols[the2_dd]) # dz2dt = the2_dd
dthe1dt_f = smp.lambdify(the1_d, the1_d) # z1 = the1_d
dthe2dt_f = smp.lambdify(the2_d, the2_d) # z2 = the2_d

def dSdt(S, t, g, m1, m2, L1, L2):
    the1, the2, z1, z2 = S
    return [
        dthe1dt_f(z1),
        dthe2dt_f(z2),
        dz1dt_f(t,g,m1,m2,L1,L2,the1,the2,z1,z2),
        dz2dt_f(t,g,m1,m2,L1,L2,the1,the2,z1,z2)
    ]

# set initial conditions

t = np.linspace(0,40,1000)
g = 9.81
m1 = 1
m2 = 2
L1 = 1
L2 = 1
y0 = (1, 2.3, 0, 0) # the1, the2, z1, z2

# numerically solve ODE

res = odeint(dSdt, y0 = y0, t=t, args=(g,m1,m2,L1,L2)).T # res[0] contains the1, res[1] contains the2

# set Cartesian Coordinates

x_1 = L1*np.sin(res[0])
y_1 = -L1*np.cos(res[0])
x_2 = L1*np.sin(res[0]) + L2*np.sin(res[1])
y_2 = -L1*np.cos(res[0]) -L2*np.cos(res[1])

# set up the figure

fig, ax = plt.subplots()
ax.set_xlim(-5,5)
ax.set_ylim(-5,5)
line1 = ax.plot([],[],'ro--', markersize = 8)[0]

# define animation function

def Update(frame):
    line1.set_data([0,x_1[frame],x_2[frame]], [0,y_1[frame],y_2[frame]])
    return line1,

# animate

anim = animation.FuncAnimation(fig,Update,1000, interval = 30)
# anim.save('dblpend.gif', writer='pillow',fps=25)
plt.show()
