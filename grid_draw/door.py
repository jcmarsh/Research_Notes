import Image
import ImageDraw
import sys

file_name = sys.argv[1]
print "File name: %s" % (file_name)

max_x = 500
max_y = 500
start_margin = 100
line_width = 64
robot_width = 16


im = Image.new("1", (max_x, max_y))

draw = ImageDraw.Draw(im)

# Draw starting area (lower left corner)
draw.rectangle((0, max_y, start_margin, max_y - start_margin), fill=1)
# Draw ending area (upper right corner)
draw.rectangle((max_x - start_margin, 0, max_x, start_margin), fill=1)

# Draw the actual path to the goal, across the bottom and then right hand side
draw.rectangle((0, max_y, max_x, max_y - line_width), fill=1)
draw.rectangle((max_x, 0, max_x - line_width, max_y), fill=1)

# Draw the promising dead end.
draw.rectangle((start_margin, max_y - line_width, start_margin + line_width, max_y - 3 * line_width), fill=1)
draw.rectangle((start_margin + line_width, max_y - 3 * line_width, max_x, max_y - 2 * line_width), fill=1)

# Slot for the door robot.
draw.rectangle((start_margin+line_width, max_y-3*line_width, start_margin+line_width+robot_width, max_y-4*line_width), fill=1)

# Create an obstacle in the upper left corner and the lower right corner
# This is needed to keep player stage from contracting the map.
draw.point((0,0), fill=0)
draw.point((max_x - 1, max_y - 1), fill=0)

im.save(file_name, "PNG")
