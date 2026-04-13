#! python3
"""
Graphical representation of approximation of Pi using Monte Carlo methods.
"""
import matplotlib.pyplot as plt
import numpy as np

from SimpleImplementation import estimatePi

def main():
    samples = int(input("Enter Intended Samples:\n"))
    
    x = np.random.uniform(-1, 1, samples)
    y = np.random.uniform(-1, 1, samples)
    
    plt.plot(x, y, ".k", markersize = 1)
    
    t = np.linspace(0, 2 * np.pi, 400)
    xc = np.cos(t)
    yc = np.sin(t)
    plt.plot(xc, yc, 'r-', linewidth=1)
    plt.gca().set_aspect('equal', adjustable='box')
    
    plt.xlim(-1.1, 1.1)
    plt.ylim(-1.1, 1.1)
    plt.xlabel("x")
    plt.ylabel("y")
    
    plt.suptitle(f"Estimate of Pi: {estimatePi(samples)}", fontsize = 18)
    plt.title("Graphical Representation Is Independent To Estimate Calculation", fontsize = 10)
    plt.show()

if __name__ == "__main__":
    main()