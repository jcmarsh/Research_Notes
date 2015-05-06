import sys
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) < 1:
	print "Usage: python display_runs.py <file_0> ... <file_n>"
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
	x.append([])
	y.append([])
	for line in data_file:
		if line[0] == '(' and line[-2] == ')':
			try:
				value = int(line[1:-2])
				x[file_num].append(value / 3092.0)
				y[file_num].append(n)
				n = n + 1
			except ValueError:
				print "Failed to interpret line: ", line
		else:
			print "Poorly formatted line: ", line
	file_num = file_num + 1
	data_file.close()



plt.figure(figsize=(12, 12))

#n, bins, patches = plt.hist(x, 150, facecolor='g')

#n, bins, patches = plt.hist(x, 150, [0, 100000], facecolor='g')
#n, bins, patches = plt.hist(x, 100, facecolor='g')
lines = []
line_types = ['b.', 'r.', 'g.', 'm.', 'k.']

for i in range(0, len(datas)):
	plt.plot(y[i], x[i], line_types[i])

plt.xlabel('Command Loop')
plt.ylabel('Time in Microseconds')
plt.grid(True)

plt.show()
