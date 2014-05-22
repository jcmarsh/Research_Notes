import sys
import numpy as np
import matplotlib.pyplot as plt

print "Opening: ", sys.argv[1]
data_file = open(sys.argv[1], "r")

print data_file.readline()

x = []

for index in range(0, 100000):
    x.append(int(data_file.readline()))

print max(x)
x.remove(max(x))
print max(x)
x.remove(max(x))

plt.figure(figsize=(12, 5))

#n, bins, patches = plt.hist(x, 150, facecolor='g')

#n, bins, patches = plt.hist(x, 150, [0, 100000], facecolor='g')
n, bins, patches = plt.hist(x, 150, facecolor='g')

plt.xlabel('Time in Cycles')
plt.ylabel('Count')
plt.grid(True)

plt.show()

