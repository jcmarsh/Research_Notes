import matplotlib.pyplot as plt
from random import choice

class RandomWalk:
    """Take a walk."""

    def __init__(self, num_points=50_000):
        """Init the walk."""
        self.num_points = num_points

        # Walks start at the origin
        self.x_values = [0]
        self.y_values = [0]

    def fill_walk(self):
        """Calculate the walk"""

        def gen_step():
            direction = choice([1, -1])
            distance = choice([0, 1, 2, 3, 4, 5])
            return distance * direction

        # Take steps until it's over
        while(len(self.x_values) < self.num_points):
            x_step = gen_step()
            y_step = gen_step()

            if x_step == 0 and y_step == 0:
                continue

            self.x_values.append(self.x_values[-1] + x_step)
            self.y_values.append(self.y_values[-1] + y_step)

rw = RandomWalk()
rw.fill_walk()

plt.style.use('dark_background')
fig, ax = plt.subplots()
#color_values = [x + y for x, y in zip(rw.x_values, rw.y_values)]
color_values = range(rw.num_points)
ax.scatter(rw.x_values, rw.y_values, s=1, c=color_values, cmap=plt.cm.Blues, alpha=0.2)

ax.set_title("Random Numbers")
ax.set_xlabel("X Value", fontsize=14)
ax.set_ylabel("Y Value", fontsize=14)
ax.tick_params(axis='both', labelsize=14)
ax.grid(linestyle = '--', linewidth=0.5)

plt.show()
        