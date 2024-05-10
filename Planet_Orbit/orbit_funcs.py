import numpy as np


def distance(phi,p,e):
    return p/(1+e*np.cos(phi))

def position(phi,p,e):
    x = distance(phi,p,e)*np.cos(phi)
    y = distance(phi,p,e)*np.sin(phi)
    return x,y
