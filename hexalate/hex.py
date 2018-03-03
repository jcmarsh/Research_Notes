import ConfigParser
import sys
import numpy as np
import matplotlib.pyplot as plt
from PIL import Image


if len(sys.argv) < 1:
	print "Usage: python hex.py config.ini"
	exit()

print "Opening: ", sys.argv[1]

config_file = sys.argv[1]
my_config = ConfigParser.ConfigParser()
my_config.readfp(open(config_file)) # TODO: error check

file_name = my_config.get("image", "file_name")
width = int(my_config.get("image", "width"))
height = int(my_config.get("image", "height"))

# convert file from jpg to some bit format.
image = Image.open(file_name)

#image.show()

print(image.getdata())
datas = list(image.getdata())

print "Width:", width, " Height:", height
print "im width / height", image.size

n = 0
x = []
y = []

channel = int(my_config.get("processing", "channel"))
levels = int(my_config.get("processing", "levels"))

for l in range(levels - 1):
        print "threshold:", l, " is", ((256 * 3) / levels) * (l + 1)

for h in range(0, height):
        for w in range(0, width):
                pixel = datas[(width * h) + w]
                if channel==4:
                        for l in range(levels - 1):
                                if (pixel[0] + pixel[1] + pixel[2]) < ((256 * 3) / levels) * (l + 1):
                                        x.append(w)
                                        y.append(height - h)
                else:
                        for l in range(levels - 1):
                                if (pixel[channel] < (256 / levels) * (l + 1)):
                                        x.append(w)
                                        y.append(height - h)


# Try to get rid of axes
fig = plt.figure(frameon=False)
#fig.set_size_pixels(width, height)
ax = plt.Axes(fig, [0., 0., 1., 1.])
ax.set_axis_off()
fig.add_axes(ax)

#plt.figure(figsize=((width / 300), (height / 300)))

out_file = my_config.get("output", "out_file")
grid_size = int(my_config.get("output", "grid_size"))
heat_map = my_config.get("output", "heat_map")

plt.hexbin(x, y, gridsize=grid_size, cmap=heat_map) #'gray_r')
#plt.axis([0, width, 0, height])

fig.savefig(out_file, dpi=300)
#fig.savefig(out_file, bbox_inches='tight', dpi=300)
#plt.savefig(out_file, bbox_inches='tight', dpi=300)

plt.show()
