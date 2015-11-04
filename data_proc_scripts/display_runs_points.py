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
				value = int(line[sta_paren + 1:end_paren])
				x[file_num].append(value)
				y[file_num].append(n)
				total = total + (value);
				n = n + 1
			except ValueError:
				print "Failed to interpret line: ", line
		else:
			print "Poorly formatted line: ", line
	last = file_num;
	file_num = file_num + 1
	data_file.close()
	print "Count  : ", n
	print "Average: ", total / n
	print "Std dev: ", np.std(x[last])
	print "Maximum: ", max(x[last])
	z = np.polyfit(y[last], x[last], 1)
	print "fit slope   : ", z[0]
	print "| ", n, " | ", total / n, " | ", np.std(x[file_num - 1]), " | ", max(x[file_num - 1]), " | ", z[0], " |"

#plt.figure(figsize=(12, 12))
plt.figure(figsize=(6, 5))

#n, bins, patches = plt.hist(x, 150, facecolor='g')

#n, bins, patches = plt.hist(x, 150, [0, 100000], facecolor='g')
#n, bins, patches = plt.hist(x, 100, facecolor='g')
lines = []
line_types = ['b.', 'r.', 'g.', 'm.', 'k.', 'c.', 'y.']
tren_types = ['r', 'g', 'm', 'k', 'c', 'y', 'b']


for i in range(0, len(datas)):
	plt.plot(y[i], x[i], line_types[i])
	# calc the trendline
	z = np.polyfit(y[i], x[i], 1)
	p = np.poly1d(z)
	plt.plot(y[i], p(y[i]), tren_types[i])

plt.xlabel('Restart Count')
plt.ylabel('Time in Microseconds')
plt.grid(True)

plt.show()
