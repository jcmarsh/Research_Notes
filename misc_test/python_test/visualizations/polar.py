"""
Demo of bar plot on a polar axis.
"""
import numpy as np
import matplotlib.pyplot as plt

N=4
#N = 8
theta = np.linspace(0.0, 2 * np.pi, N, endpoint=False)
radii = {135.9555516378, 151.6977794622, 3851.11998298, 62.9688898311}
width = np.pi / 2

#theta[1] = 0.0
#theta[3] = 0.5 * np.pi
#theta[5] = np.pi
#theta[7] = 1.5 * np.pi
#theta = {   0.0,          0.0, 0.5 * np.pi,               0.5 * np.pi,      np.pi,                         np.pi, 1.5 * np.pi,                1.5 * np.pi}
#radii = {243.08, 243.08 + 244,  162.053196, 162.053196 + 292.32000576, 397.973472, 397.973472 + 10233.0170827556,  277.333056, 277.333056 + 157.440005376}



ax = plt.subplot(111, projection='polar')
bars = ax.bar(theta, radii, width=width, bottom=1)


# Use custom colors and opacity
dark = True
for r, bar in zip(radii, bars):
        if dark:
                bar.set_facecolor(plt.cm.jet(r / 10.))
                bar.set_alpha(0.5)
        else:
                bar.set_facecolor(plt.cm.jet(r / 100.))
                bar.set_alpha(0.1)
        dark = not dark
                
                
#ax.set_rlim(0)
#ax.set_rscale('log')
plt.ylim(1, 10000.0)
plt.yscale('log')

plt.show()
