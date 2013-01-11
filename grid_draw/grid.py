import Image
import ImageDraw
import sys

file_name = sys.argv[1]
line_width = int(sys.argv[2])
print "File name: %s\t Alley width: %d" % (file_name, line_width)

max_x = 500
max_y = 500
start_margin = 100
alley_number = 4

im = Image.new("1", (max_x, max_y))

draw = ImageDraw.Draw(im)

# Draw starting area (lower left corner)
draw.rectangle((0, max_y, start_margin, max_y - start_margin), fill=1)
# Draw ending area (upper right corner)
draw.rectangle((max_x - start_margin, 0, max_x, start_margin), fill=1)

# Vertical Alleys, 1st and last
draw.rectangle((0, 0, line_width, max_y), fill=1)
draw.rectangle((max_x - line_width, 0, max_x, max_y), fill=1)
# Infil
inner_width = max_x - (2 * line_width)
x_value = line_width
x_step = ((inner_width - (line_width * (alley_number - 2))) / (alley_number - 1))
for i in range(0, alley_number - 2):
    x_value = x_value + x_step
    draw.rectangle((x_value, 0, x_value + line_width, max_y), fill=1)
    x_value = x_value + line_width

# Horizontal Alleys, 1st and last
draw.rectangle((0, 0, max_x, line_width), fill=1)
draw.rectangle((0, max_y - line_width, max_x, max_y), fill=1)
# Infil
inner_height = max_y - (2 * line_width)
y_value = line_width
y_step = ((inner_height - (line_width * (alley_number - 2))) / (alley_number - 1))
for i in range(0, alley_number - 2):
    y_value = y_value + y_step
    draw.rectangle((0, y_value, max_x, y_value + line_width), fill=1)
    y_value = y_value + line_width

# Create an obstacle in the upper left corner and the lower right corner
# This is needed to keep player stage from contracting the map.
draw.point((0,0), fill=0)
draw.point((max_x - 1, max_y - 1), fill=0)

im.save(file_name, "PNG")
