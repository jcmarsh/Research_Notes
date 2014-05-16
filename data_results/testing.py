import numpy as np
import matplotlib.pyplot as plt

data_file = open("./TimingTicks/HundredThousandInterruptions_new.txt", "r")

print data_file.readline()

x = []

for index in range(0, 100000):
    x.append(int(data_file.readline()))

#mu, sigma = 100, 15
#x = mu + sigma * np.random.randn(10000)

plt.figure(figsize=(12, 5))

#n, bins, patches = plt.hist(x, 150, facecolor='g')

n, bins, patches = plt.hist(x, 150, [0, 100000], facecolor='g')

plt.xlabel('Time in Cycles')
plt.ylabel('Count')
plt.grid(True)

print max(x)
x.remove(max(x))
print max(x)

plt.show()

