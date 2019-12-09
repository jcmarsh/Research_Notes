import matplotlib.pyplot as plt
import random

max_num = 10_000

x_values = [random.randint(0, value) for value in range(1, max_num)]
y_values = [random.randint(0, value) for value in range(1, max_num)]

plt.style.use('dark_background')
fig, ax = plt.subplots()
ax.scatter(x_values, y_values, s=4, alpha=0.2)

ax.set_title("Random Numbers")
ax.set_xlabel("X Value", fontsize=14)
ax.set_ylabel("Y Value", fontsize=14)
ax.axis([0, max_num, 0, max_num])
ax.tick_params(axis='both', labelsize=14)
ax.grid(linestyle = '--', linewidth=0.5)

plt.show()
 