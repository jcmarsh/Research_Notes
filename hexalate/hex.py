import sys
import numpy as np
import matplotlib.pyplot as plt
from PIL import Image


if len(sys.argv) < 3:
	print "Usage: python hex.py <image_file.jpg> <width> <height>"
	exit()

print "Opening: ", sys.argv[1], "\tSize: ", sys.argv[2], ", ", sys.argv[3]

width = int(sys.argv[2])
height = int(sys.argv[3])

# convert file from jpg to some bit format.
image = Image.open(sys.argv[1])

#image.show()

print(image.getdata())
datas = list(image.getdata())

print "Width:", width, " Height:", height
print "im width / height", image.size

n = 0
x = []
y = []

for h in range(0, height):
        for w in range(0, width):
                pixel = datas[(width * h) + w]
                #print w, h, pixel
                if (pixel[0] + pixel[1] + pixel[2]) < 500:
                        x.append(w)
                        y.append(height - h)
                if (pixel[0] + pixel[1] + pixel[2]) < 350:
                        x.append(w)
                        y.append(height - h)
                if (pixel[0] + pixel[1] + pixel[2]) < 200:
                #if (pixel[0]) < 128:
                        #print(w, h, pixel[1])
                        x.append(w)
                        #if (datas[(h * height) + w][1]) > 128:
                        y.append(height - h)
                #else:
                        #print("Didn't make it", w, h, pixel[1])

#plt.figure(figsize=(width, height))
#plt.figure(figsize=((width / 300), (height / 300)))

plt.hexbin(x, y, gridsize=80, cmap='gist_heat_r') #'gray_r')
plt.axis([0, width, 0, height])

plt.savefig("out.png", bbox_inches='tight') #, dpi=100)

plt.show()
