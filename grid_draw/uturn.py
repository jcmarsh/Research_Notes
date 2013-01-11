import Image
import ImageDraw
import sys

file_name = sys.argv[1]
line_width = int(sys.argv[2])
print "File name: %s\t Alley width: %d" % (file_name, line_width)

max_x = 500
max_y = 500
start_margin = 100

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
draw.rectangle((start_margin, start_margin, start_margin + line_width, max_y - line_width), fill=1)
draw.rectangle((start_margin, start_margin, max_x - 2 * start_margin, start_margin + line_width), fill=1)

# Create an obstacle in the upper left corner and the lower right corner
# This is needed to keep player stage from contracting the map.
draw.point((0,0), fill=0)
draw.point((max_x - 1, max_y - 1), fill=0)

im.save(file_name, "PNG")
