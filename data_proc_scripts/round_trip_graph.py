#!/usr/bin/env python
# Round trip communication times.
import numpy as np
import matplotlib.pyplot as plt

p_color = '#e66101'
s_color = '#b2abd2'
t_color = '#f7f7f7'

N = 4

resScale = ('NMR', 'SMR', 'DMR', 'TMR')
resMeans_0 = (14.0143565888, 27.3617210989, 40.7126991814, 53.2786750175)
resStd_0 = (0.249131762173, 0.515115130363, 0.724440297815, 0.911442091626)
resLong_0 = (19.7205692109, 40.5924967658, 58.7490297542, 70.1151358344)

resMeans_1024 = (14.1330857409, 27.5906206706, 41.1368818735, 53.8474723922)
resStd_1024 = (0.284471487839, 0.45524863988, 0.56048467411, 0.894289372705)
resLong_1024 = (20.3699870634, 41.6222509702, 55.4243208279, 74.2328589909)

resMeans_2048 = (14.2284494055, 28.0523994424, 42.098655283, 55.2895733332)
resStd_2048 = (0.279101443178, 0.456467842508, 0.659214846756, 1.04638394039)
resLong_2048 = (21.3557567917, 39.107373868, 60.9249676585, 86.1785252264)


ind = np.arange(N)  # the x locations for the groups
width = 0.3       # the width of the bars

fig, ax = plt.subplots() #figsize=(6, 5))

#ax.xaxis.get_ticklabels().set_fontsize(10)
#ax.yaxis.ticklabels.set_fontsize(10)

rects2 = ax.bar(ind + .05 + 0 * width, resLong_0, width, color=s_color, label='WCET 84B')
rects1 = ax.bar(ind + .05 + 0 * width, resMeans_0, width, color=p_color, label='Mean 84B', yerr=resStd_0,  error_kw=dict(elinewidth=8, ecolor=t_color, capsize=10))

rects4 = ax.bar(ind + .05 + 1 * width, resLong_1024, width, color=s_color, label='WCET 1024B', hatch='..')
rects3 = ax.bar(ind + .05 + 1 * width, resMeans_1024, width, color=p_color, label='Mean 1024B', yerr=resStd_1024, hatch='..', error_kw=dict(elinewidth=8, ecolor=t_color, capsize=10))

rects6 = ax.bar(ind + .05 + 2 * width, resLong_2048, width, color=s_color, label='WCET 2048B', hatch='//\\\\')
rects5 = ax.bar(ind + .05 + 2 * width, resMeans_2048, width, color=p_color, label='Mean 2048B', yerr=resStd_2048, hatch='//\\\\', error_kw=dict(elinewidth=8, ecolor=t_color, capsize=10))


# add some text for labels, title and axes ticks
ax.set_ylabel('Time (uS)', fontsize=16)
ax.set_xlabel('Redundancy Level', fontsize=16)
ax.set_title('Round Trip Communication', fontsize=20)
ax.set_xticks(ind+.5)
ax.set_xticklabels(resScale)
ax.tick_params(labelsize=18, pad=8)

#ax.legend( (rects1[0], rects2[0]), ('Mean', 'WCET'), loc='best' )
#ax.legend(loc='best')
ax.legend(loc='upper left')


#def autolabel(rects):
#    # attach some text labels
#    for rect in rects:
#        height = rect.get_height()
#        ax.text(rect.get_x()+rect.get_width()/2., 1.05*height, '%d'%int(height),
#                ha='center', va='bottom')

#autolabel(rects1)
#autolabel(rects2)

plt.show()