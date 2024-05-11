import numpy as np


# Elastic potentil at distance x from equilibrium
def ElasticPotential(x,k):
    return 0.5*k*x**2

# Kinetic Energy of a unitary mass
def KineticEnergy(v):
    return 0.5*v**2

# Calculate coefficients of Solution: x = Ccos(w*t) + Ssin(w*t)
def CoefficientsOfSolution(x0,v0,t0,k):
    w = np.sqrt(k)
    # A[C, S] = B
    # B = [x0, v0]
    A = np.array([[np.cos(w*t0) , np.sin(w*t0)], [-w*np.sin(w*t0) , w*np.cos(w*t0)]])
    B = np.array([x0, v0])
    # sol = [C, S] = A^(-1)B
    sol = np.linalg.inv(A).dot(B)

    return sol[0],sol[1],

def Position(k,x0,v0,t0,t):
    w = np.sqrt(k)
    (C, S) =  CoefficientsOfSolution(x0,v0,t0,k)
    x = C*np.cos(w*t) + S*np.sin(w*t)
    return x

def Velocity(k,x0,v0,t0,t):
    w = np.sqrt(k)
    (C, S) =  CoefficientsOfSolution(x0,v0,t0,k)
    v = -w*C*np.sin(w*t)+w*S*np.cos(w*t)
    return v


