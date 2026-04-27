#! Python3
"""
Lennard-Jones potential plotted in Matplotlib 
"""
import numpy as np
import matplotlib.pyplot as plt
from typing import final


def main():
    r = np.linspace(0, 3.0, 1000)
    SIGMA: final = 1.0
    EPSILON: final = 1.0
    V = 4 * EPSILON * ((SIGMA/r)**12 - (SIGMA/r)**6) 

    plt.plot(r, V)
    
    plt.ylim(-1.2, 2)
    plt.xlim(0.9, 3)
    plt.grid(True)
    
    plt.show()

if __name__ == "__main__":
    main()