import numpy as np
import matplotlib.pyplot as plt

def graph(formula, x_range):
    x = np.array(x_range)
    t_values = [1.96, 2.5758]
    e_values = [0.01]

    for t in t_values:
        for e in e_values:
            y = formula(x, t, e)
            max_y = max(y)
            for index,p in enumerate(x):
                if (p > 0.5):
                    y[index] = max_y
            plt.plot(x, y)
    plt.savefig("p_hat.svg")
    plt.show()

def leveugle(p, t, e):
    # t = 1.96
    # e = 0.01
    N = 100000000000

    return N / (1 + (e * e) * ( (N - 1) / (t * t * p * ( 1 - p))))

graph(leveugle, np.arange(0, 1, 0.01))
