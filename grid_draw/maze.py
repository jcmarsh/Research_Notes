import Image
import ImageDraw
import sys


if len(sys.argv) < 3:
	print "Usage: python maze.py <file_name> <line_width>"
	sys.exit()

file_name = sys.argv[1]

max_x = 500
max_y = 500

start_margin = 100

im = Image.new("1", (max_x, max_y))

draw = ImageDraw.Draw(im)

# Draw starting area (lower left corner)
#draw.rectangle((0, max_y, start_margin, max_y - start_margin), fill=1)
# Draw ending area (upper right corner)
#draw.rectangle((max_x - start_margin, 0, max_x, start_margin), fill=1)

# The Maze
#   ########..  
#   #...###...
#   #.#.###.##
#   #...#.#.##
#   #.###.#.#.
#   #.....#.#.
#   #.#####...
#   #.##..##.#
#   ..##.###.#
#   .........#   

maze = []
maze.append([1, 1, 1, 1, 1, 1, 1, 1, 0, 0])
maze.append([1, 0, 0, 0, 1, 1, 1, 0, 0, 0])
maze.append([1, 0, 1, 0, 1, 1, 1, 0, 1, 1])
maze.append([1, 0, 0, 0, 1, 0, 1, 0, 1, 1])
maze.append([1, 0, 1, 1, 1, 0, 1, 0, 1, 0])
maze.append([1, 0, 0, 0, 0, 0, 1, 0, 1, 0])
maze.append([1, 0, 1, 1, 1, 1, 1, 0, 0, 0])
maze.append([1, 0, 1, 1, 0, 0, 1, 1, 0, 1])
maze.append([0, 0, 1, 1, 0, 1, 1, 1, 0, 1])
maze.append([0, 0, 0, 0, 0, 0, 0, 0, 0, 1])

for i in range(0, len(maze)):
	for j in range(0, len(maze[i])):
		if (maze[i][j] == 0):
			#print "clearing out", j * 50, i * 50, j * 50 + 50, i * 50 + 50
			draw.rectangle((j * 50, i * 50, j * 50 + 50, i * 50 + 50), fill=1) 

im.save(file_name, "PNG")