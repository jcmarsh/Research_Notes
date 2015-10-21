#!/usr/bin/env python
# Just trying to plot the restart data
import numpy as np
import matplotlib.pyplot as plt

p_color = '#e66101'
s_color = '#b2abd2'
t_color = '#f7f7f7'

N = 7
# resScale = (1092, 2048, 3072, 4096, 6144, 8192, 12288, 16384, 24576, 32768, 49152, 65536)
#resMeans = (430.551546884, 948.5191319, 1504.22759579, 2053.20151607, 3136.15862416, 4252.51205499, 6459.93441162, 8697.05646045, 13253.1997097, 17850.4045186, 27490.3359711, 39039.0927964)
#resStd = (6.14559630679, 23.6156672081, 51.0110813798, 89.2758497886, 145.543755155, 266.992815844, 462.487202412, 716.648668492, 1330.46536788, 1990.25059109, 3535.79848568,  6024.9330016)
#resLong = (556.895213454, 1257.15135834, 1997.34282018, 2734.29366106, 4181.54075032, 5643.63518758, 8552.25614489, 11451.1267788, 17250.2639069, 23060.8913325, 42313.0258732, 53773.7930142)
resScale = (r'$2^{10}$', r'$2^{11}$', r'$2^{12}$', r'$2^{13}$', r'$2^{14}$', r'$2^{15}$', r'$2^{16}$')
resMeans = (430.551546884, 948.5191319, 2053.20151607, 4252.51205499, 8697.05646045, 17850.4045186, 39039.0927964)
resStd = (6.14559630679, 23.6156672081, 89.2758497886, 266.992815844, 716.648668492, 1990.25059109,  6024.9330016)
resLong = (556.895213454, 1257.15135834,2734.29366106, 5643.63518758, 11451.1267788, 23060.8913325, 53773.7930142)

ind = np.arange(N)  # the x locations for the groups
width = 0.8       # the width of the bars

fig, ax = plt.subplots() #figsize=(6, 5))

#ax.xaxis.get_ticklabels().set_fontsize(10)
#ax.yaxis.ticklabels.set_fontsize(10)

rects2 = ax.bar(ind + .1, resLong, width, color=s_color, log=True, label='WCET')
rects1 = ax.bar(ind + .1, resMeans, width, color=p_color, log=True, label='Mean', yerr=resStd,  error_kw=dict(elinewidth=8, ecolor=t_color, capsize=10))

# add some text for labels, title and axes ticks
ax.set_ylabel('Time (uS)', fontsize=16)
ax.set_xlabel('Component Size (Kb)', fontsize=16)
ax.set_title('Restart of Components', fontsize=20)
ax.set_xticks(ind+.5)
ax.set_xticklabels(resScale)
ax.tick_params(labelsize=18, pad=8)

#ax.legend( (rects1[0], rects2[0]), ('Mean', 'WCET'), loc='best' )
ax.legend(loc='best' )


#def autolabel(rects):
#    # attach some text labels
#    for rect in rects:
#        height = rect.get_height()
#        ax.text(rect.get_x()+rect.get_width()/2., 1.05*height, '%d'%int(height),
#                ha='center', va='bottom')

#autolabel(rects1)
#autolabel(rects2)

plt.show()