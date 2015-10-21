#!/usr/bin/env python
# Graph task performance
import numpy as np
import matplotlib.pyplot as plt

p_color = '#e66101'
s_color = '#b2abd2'
t_color = '#f7f7f7'

N = 1 # it's a pain, but I don't know of a better way to get the spacing perfect

npTime    = [120.864964323]
npTimeSD  = [8.64655827689]
npPed     = [0.315313123968]
npPedSD   = [0.00299229990781]

smrTime   = [119.89283673]
smrTimeSD = [6.8491045929]
smrPed    = [0.318644041431]
smrPedSD  = [0.00318675500116]

tmrTime   = [130.57420391, 147.986675935, 151.661494208]
tmrTimeSD = [4.9594513163, 7.83554122431, 9.34067631268]
tmrPed    = [0.364823480493, 0.375723251136, 0.451915190392]
tmrPedSD  = [0.00768190040292, 0.00569795048298, 0.0121496291507]

co1Time   = [127.475672182]
co1Time_exec = [133.040182316]
co1Time_ctrl = [133.556262686]
co1TimeSD = [5.8531381894]
co1TimeSD_exec = [4.84440317817]
co1TimeSD_ctrl = [5.64760736197]
co1Ped    = [0.342960003812]
co1Ped_exec = [0.351669415919]
co1Ped_ctrl = [0.402821981749]
co1PedSD  = [0.0053250998167]
co1PedSD_exec = [0.00358014765758]
co1PedSD_ctrl = [0.00847807180767]

co2Time   = [120.79047348]
co2Time_exec = [135.832141568]
co2Time_ctrl = [138.643300419]
co2TimeSD = [3.71654164643]
co2TimeSD_exec = [5.31789449456]
co2TimeSD_ctrl = [4.70322615541]
co2Ped    = [0.346533840087]
co2Ped_exec = [0.348205867926]
co2Ped_ctrl = [0.406930503857]
co2PedSD  = [0.00360606452266]
co2PedSD_exec = [0.00380313787543]
co2PedSD_ctrl = [0.00915038794867]

for i in range(0, N):
	npTime[i]   = tmrTime[i]   / npTime[i]
	npTimeSD[i] = tmrTimeSD[i] / npTimeSD[i]
	npPed[i]    = tmrPed[i]    / npPed[i]
	npPedSD[i]  = tmrPedSD[i]  / npPedSD[i]

	smrTime[i]   = tmrTime[i]   / smrTime[i]
	smrTimeSD[i] = tmrTimeSD[i] / smrTimeSD[i]
	smrPed[i]    = tmrPed[i]    / smrPed[i]
	smrPedSD[i]  = tmrPedSD[i]  / smrPedSD[i]

	co1Time[i]   = tmrTime[0]   / co1Time[i]
	co1TimeSD[i] = tmrTimeSD[0] / co1TimeSD[i]
	co1Ped[i]    = tmrPed[0]    / co1Ped[i]
	co1PedSD[i]  = tmrPedSD[0]  / co1PedSD[i]

	co1Time_exec[0]   = tmrTime[1]   / co1Time_exec[0]
	co1TimeSD_exec[0] = tmrTimeSD[1] / co1TimeSD_exec[0]
	co1Ped_exec[0]    = tmrPed[1]    / co1Ped_exec[0]
	co1PedSD_exec[0]  = tmrPedSD[1]  / co1PedSD_exec[0]

	co1Time_ctrl[0]   = tmrTime[2]   / co1Time_ctrl[0]
	co1TimeSD_ctrl[0] = tmrTimeSD[2] / co1TimeSD_ctrl[0]
	co1Ped_ctrl[0]    = tmrPed[2]    / co1Ped_ctrl[0]
	co1PedSD_ctrl[0]  = tmrPedSD[2]  / co1PedSD_ctrl[0]

	co2Time[0]   = tmrTime[0]   / co2Time[0]
	co2TimeSD[0] = tmrTimeSD[0] / co2TimeSD[0]
	co2Ped[0]    = tmrPed[0]    / co2Ped[0]
	co2PedSD[0]  = tmrPedSD[0]  / co2PedSD[0]

	co2Time_exec[0]   = tmrTime[1]   / co2Time_exec[0]
	co2TimeSD_exec[0] = tmrTimeSD[1] / co2TimeSD_exec[0]
	co2Ped_exec[0]    = tmrPed[1]    / co2Ped_exec[0]
	co2PedSD_exec[0]  = tmrPedSD[1]  / co2PedSD_exec[0]

	co2Time_ctrl[0]   = tmrTime[2]   / co2Time_ctrl[0]
	co2TimeSD_ctrl[0] = tmrTimeSD[2] / co2TimeSD_ctrl[0]
	co2Ped_ctrl[0]    = tmrPed[2]    / co2Ped_ctrl[0]
	co2PedSD_ctrl[0]  = tmrPedSD[2]  / co2PedSD_ctrl[0]

#mazeTime = (120.864964323, 119.89283673, 130.57420391, 127.475672182, 120.79047348, 147.986675935, 133.040182316, 135.832141568, 151.661494208, 133.556262686, 138.643300419, 126.402624875)
#mazeSD = (8.64655827689, 6.8491045929, 4.9594513163, 5.8531381894, 3.71654164643, 7.83554122431, 4.84440317817, 5.31789449456, 9.34067631268, 5.64760736197, 4.70322615541, 3.20518799137)
#pedTime = (3.17173686435, 3.13861401366, 2.74225478255, 2.9164884029, 2.88603362421, 2.66214642372, 2.84387512739, 2.8722082122, 2.21442183862, 2.4835933242, 2.45864721786, 2.63016463205)
#pedSD = (0.0301657538491, 0.0315890363317, 0.0571587961481, 0.0448659965628, 0.030038851009, 0.0404259467768, 0.029024286598, 0.0314907916725, 0.0601900233828, 0.0526269761635, 0.0545205111948, 0.0497525229769)

ind = np.arange(N)  # the x locations for the groups
width = 0.10       # the width of the bars

fig, ax = plt.subplots() #figsize=(6, 5))

#ax.xaxis.get_ticklabels().set_fontsize(10)
#ax.yaxis.ticklabels.set_fontsize(10)

p = np.poly1d(1)
r = np.arange(ind + 1.1 + 14 * width)
plt.plot(r, p(r), 'r')

# First group, no faults injected
#rects02 = ax.bar(ind + .1 + 0 * width,  npPed, width, color=s_color, label='NMR Maze')
#rects01 = ax.bar(ind + .1 + 1 * width,  npTime, width, color=p_color, label='NMR Ped')
#rects04 = ax.bar(ind + .1 + 2 * width, smrPed, width, color=s_color, label='SMR Maze', hatch='..')
#rects03 = ax.bar(ind + .1 + 3 * width, smrTime, width, color=p_color, label='SMR Ped', hatch='..')
rects06 = ax.bar(ind + .1 + 0 * width, co1Ped, width, color=s_color, label=r'$Config_{1}$ Ped')
rects05 = ax.bar(ind + .1 + 1 * width, co1Time, width, color=p_color, label=r'$Config_{1}$ Maze')
rects08 = ax.bar(ind + .1 + 2 * width, co2Ped, width, color=s_color, label=r'$Config_{2}$ Ped', hatch='..')
rects07 = ax.bar(ind + .1 + 3 * width, co2Time, width, color=p_color, label=r'$Config_{2}$ Maze', hatch='..')

# second group, exec faults, just config_1 and config_2
ax.axvline(ind + .1 + 4.5 * width, color='k', linestyle='--')
rects10 = ax.bar(ind + .1 + 5 * width, co1Ped_exec, width, color=s_color)
rects09 = ax.bar(ind + .1 + 6 * width, co1Time_exec, width, color=p_color)
rects12 = ax.bar(ind + .1 + 7 * width, co2Ped_exec, width, color=s_color, hatch='..')
rects11 = ax.bar(ind + .1 + 8 * width, co2Time_exec, width, color=p_color, hatch='..')

# third group, ctrl faults, just config_1 and config_2
ax.axvline(ind + .1 + 9.5 * width, color='k', linestyle='--')
rects14 = ax.bar(ind + .1 + 10 * width, co1Ped_ctrl, width, color=s_color)
rects13 = ax.bar(ind + .1 + 11 * width, co1Time_ctrl, width, color=p_color)
rects16 = ax.bar(ind + .1 + 12 * width, co2Ped_ctrl, width, color=s_color, hatch='..')
rects15 = ax.bar(ind + .1 + 13 * width, co2Time_ctrl, width, color=p_color, hatch='..')

# add some text for labels, title and axes ticks
ax.set_ylabel('Performance (Speedup compared to TMR)', fontsize=16)
ax.set_xlabel('Faults Injected', fontsize=16)
ax.set_title('Task Performance Compared to Baseline', fontsize=20)
ax.set_xticks((ind + .1 + 2 * width, ind + .1 + 7 * width, ind + .1 + 12 * width))
ax.set_xticklabels(('None', 'Exec', 'Ctrl'))
ax.tick_params(labelsize=18, pad=8)

ax.legend(loc='lower right')
ax.set_xlim([0, ind + .2 + 14 * width])


#def autolabel(rects):
#    # attach some text labels
#    for rect in rects:
#        height = rect.get_height()
#        ax.text(rect.get_x()+rect.get_width()/2., 1.05*height, '%d'%int(height),
#                ha='center', va='bottom')

#autolabel(rects1)
#autolabel(rects2)

plt.show()