#!/usr/bin/env python
# Just trying to plot the restart data
import numpy as np
import matplotlib.pyplot as plt

N = 12
resScale = (1092, 2048, 3072, 4096, 6144, 8192, 12288, 16384, 24576, 32768, 49152, 65536)
resMeans = (430.551546884, 948.5191319, 1504.22759579, 2053.20151607, 3136.15862416, 4252.51205499, 6459.93441162, 8697.05646045, 13253.1997097, 17850.4045186, 27490.3359711, 39039.0927964)
resStd = (6.14559630679, 23.6156672081, 51.0110813798, 89.2758497886, 145.543755155, 266.992815844, 462.487202412, 716.648668492, 1330.46536788, 1990.25059109, 3535.79848568,  6024.9330016)
resLong = (556.895213454, 1257.15135834, 1997.34282018, 2734.29366106, 4181.54075032, 5643.63518758, 8552.25614489, 11451.1267788, 17250.2639069, 23060.8913325, 42313.0258732, 53773.7930142)

ind = np.arange(N)  # the x locations for the groups
width = 0.35       # the width of the bars

fig, ax = plt.subplots()

rects2 = ax.bar(ind, resLong, width, color='y', log=True)
rects1 = ax.bar(ind, resMeans, width, color='r', yerr=resStd, log=True)

# add some text for labels, title and axes ticks
ax.set_ylabel('Time (uS)')
ax.set_title('Restart of Components')
ax.set_xticks(ind+width)
ax.set_xticklabels(resScale)

ax.legend( (rects1[0], rects2[0]), ('Mean', 'WCET') )

#def autolabel(rects):
#    # attach some text labels
#    for rect in rects:
#        height = rect.get_height()
#        ax.text(rect.get_x()+rect.get_width()/2., 1.05*height, '%d'%int(height),
#                ha='center', va='bottom')

#autolabel(rects1)
#autolabel(rects2)

plt.show()