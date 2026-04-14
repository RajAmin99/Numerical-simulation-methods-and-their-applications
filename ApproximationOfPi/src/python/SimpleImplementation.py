#! python3
"""
Simple implementation of approximation of pi.

Steps:
Generate random x, y inside centre of square of side 2
Repeat steps for a large number of points
Calculate ratio of points
Multiply by 4 (pi / 4)
"""

import numpy as np

def estimatePi(samples):
    insideCircle = 0
    for i in range(samples):
        x = np.random.uniform(-1, 1)
        y = np.random.uniform(-1, 1)
        
        if (x**2 + y**2) <= 1:
            insideCircle += 1
    ratio = insideCircle / samples
    return (ratio * 4)

if __name__ == "__main__":
    print(f"Estimate with 1 Sample:\n{estimatePi(1)}\n")
    print(f"Estimate with 10 Sample:\n{estimatePi(10)}\n")
    print(f"Estimate with 50 Sample:\n{estimatePi(50)}\n")
    print(f"Estimate with 100 Sample:\n{estimatePi(100)}\n")
    print(f"Estimate with 1000 Sample:\n{estimatePi(1_000)}\n")
    print(f"Estimate with 10,000 Sample:\n{estimatePi(10_000)}\n")
    print(f"Estimate with 1,000,000 Sample:\n{estimatePi(1_000_000)}\n")
        
        