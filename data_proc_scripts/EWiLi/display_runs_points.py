import math
import sys
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) < 1:
	print "Usage: python display_runs_points.py <file_0> ... <file_n>"
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
	total = 0;
	x.append([])
	y.append([])
	for line in data_file:
		sta_paren = line.find('(')
		end_paren = line.find(')')
		if sta_paren >= 0 and end_paren > 0:
			try:
				value = float(line[sta_paren + 1:end_paren])
				x[file_num].append(value)
				y[file_num].append(len(x[file_num]))
			except ValueError:
				print "Failed to interpret line: ", line
		else:
			print "Poorly formatted line: ", line
	last = file_num;
	file_num = file_num + 1
	data_file.close()

plt.figure(figsize=(12, 10))
#plt.figure(figsize=(6, 5))

#n, bins, patches = plt.hist(x, 150, facecolor='g')

#n, bins, patches = plt.hist(x, 150, [0, 100000], facecolor='g')
#n, bins, patches = plt.hist(x, 100, facecolor='g')
lines = []
line_types = ['b.', 'r.', 'g.', 'm.', 'k.', 'c.', 'y.']
tren_types = ['r', 'g', 'm', 'k', 'c', 'y', 'b']

print "| count | average | std. dev. | std. err. | max |"
for i in range(0, len(datas)):
	# plt.plot(y[i], x[i], line_types[i])
	# calc the trendline
	z = np.polyfit(y[i], x[i], 2)
	p = np.poly1d(z)
	#plt.plot(y[i], p(y[i]))
	above_points_x = []
	above_points_y = []
	n = 0;
	for j in range(0, len(x[i])):
	#	if x[i][j] > np.polyval(z, y[i][j]):
		above_points_x.append(x[i][j])
		above_points_y.append(y[i][j])

	print "FileNam: ", sys.argv[i + 1]
	print "Count  : ", len(above_points_x)
	print "Average: ", sum(above_points_x) / len(above_points_x)
	print "Std dev: ", np.std(above_points_x)
	print "Std err: ", np.std(above_points_x) / math.sqrt(len(above_points_x))
	print "Maximum: ", max(above_points_x)
	print "| ", len(above_points_x), " | ", sum(above_points_x) / len(above_points_x), " | ", np.std(above_points_x), " | ", np.std(above_points_x) / math.sqrt(len(above_points_x)), " | ", max(above_points_x), " |"

	#plt.plot(above_points_y, above_points_x, '.')
	plt.plot(y[i], x[i], '.')
	

plt.xlabel('Restart Count')
plt.ylabel('Time in Microseconds')
plt.grid(True)

plt.show()
