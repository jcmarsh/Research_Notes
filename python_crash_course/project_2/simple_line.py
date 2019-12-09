import matplotlib.pyplot as plt

input_values = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
squares = [value ** 2 for value in input_values] # <- Love it!

#plt.style.use('fivethirtyeight')
plt.style.use('dark_background')
fig, ax = plt.subplots()
ax.plot(input_values, squares, linewidth=3)

ax.set_title("Square Numbers")
ax.set_xlabel("Value", fontsize=14)
ax.set_ylabel("Square", fontsize=14)
ax.tick_params(axis='both', labelsize=14)
ax.grid(linestyle = '--', linewidth=0.5)

plt.show()
