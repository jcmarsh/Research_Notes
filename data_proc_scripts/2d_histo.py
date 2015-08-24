import sys
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) < 1:
	print "Usage: python 2d_histo.py <file_0> ... <file_n>"
	exit()

datas = []

for i in range(1, len(sys.argv)):
	print "Opening: ", sys.argv[i]
	datas.append(open(sys.argv[i], "r"))

x = []
y = []

file_num = 0
for data_file in datas:
	n = 0
	total_vel = 0
        elapsed_time = 0.0;
        min_distances = [];
	x.append([])
	y.append([])
	for line in data_file:
		if line[0] == '(' and line[-2] == ')':
			try:
				values = line[1:-2]
				value = values.split(',');
				x[file_num].append(float(value[4]))
				y[file_num].append(float(value[5]))
				# what about mins?
                                min_distances.append(float(value[0]))
				total_vel = total_vel + float(value[1])
                                elapsed_time = elapsed_time + float(value[3])
				n = n + 1
			except ValueError:
				print "Failed to interpret line: ", line
		else:
			print "Poorly formatted line: ", line
	print "Count  : ", n
	print "Average Velocity: ", total_vel / n
	file_num = file_num + 1
        print "Time: ", elapsed_time, "\tFiles: ", file_num
        print "Avg Time elapsed: ", elapsed_time / file_num
        print "Distance Mean: ", sum(min_distances) / len(min_distances)
        print "Distance Mini: ", min(min_distances)
	data_file.close()

plt.figure(figsize=(12, 12))

for i in range(0, len(datas)):
        plt.hexbin(x[i], y[i], gridsize=80, cmap='gist_heat_r') #'gray_r')
        plt.axis([-8, 8, -8, 8])

        #heatmap, xedges, yedges = np.histogram2d(x[i], y[i], bins=25)
        #extent = [xedges[0], xedges[-1], yedges[0], yedges[-1]]
        #extent = [-8, 8, -8, 8]

        #plt.clf()
        #plt.imshow(heatmap, extent=extent)

#plt.grid(True)

plt.show()
