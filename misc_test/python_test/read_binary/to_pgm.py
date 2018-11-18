import sys

# Take an ascii text file and convert it to a .pgm

if len(sys.argv) < 3:
	print("Usaged: python3 to_pgm.py <file.txt> <output_file.pgm")
	exit()

print("Opening: ", sys.argv[1])

txt_file = open(sys.argv[1], "r")
pgm_file = open(sys.argv[2], "wb")

# Read the header info

# Not dealing with comments in header (start with '#')
# P5    <- magic header (binary grey scale
# 76 95 <- x_size, y_size
# 255   <- max value (0 to 255)

# Followed by x_size * y_size characters
# In a binary file, these would just by bytes. Here we have to deal with some ascii bytes
# as well as escaped binary bytes such as b'\xff'

char1 = txt_file.read(1)
char2 = txt_file.read(1)

if ((char1 != 'P') or (char2 != '5')):
        print("Not a pgm file")
        sys.exit()

txt_file.read(1) # advance to next line
        
next_line = txt_file.readline()
sizes = next_line.split()

try:
        x_size = int(sizes[0])
        y_size = int(sizes[1])
except ValueError:
        print("Couldn't read sizes", next_line)
        sys.exit()

next_line = txt_file.readline()

try:
        max_value = int(next_line)
except ValueError:
        print("Couldn't read max value", next_line)
        sys.exit()

print("PGM File with size:", str(x_size), str(y_size))

pgm_file.write((char1 + char2 + '\n').encode('utf-8'))
pgm_file.write((str(x_size) + " " + str(y_size) + "\n").encode('utf-8'))
pgm_file.write((str(max_value) + "\n").encode('utf-8'))

for line in txt_file:
        print("Oh, hi Mark.")
        start_index = 0
        edit_index = line.find("b\'\\x")
        print("Hey there:", edit_index)

        output_line = bytearray(0)

        while (edit_index >= 0):
                print("Indexes: ", start_index, edit_index)
                print("\tThat section: ", line[start_index:edit_index])
                output_line = output_line + line[start_index:edit_index].encode('utf-8')
                print("\tFunny chars : ", line[edit_index:edit_index + 7])
                output_line = output_line + bytes([int(line[edit_index+4:edit_index+6], 16)])
                print("The bytes: ", str(bytes([int(line[edit_index+4:edit_index+6], 16)])))
                start_index = edit_index + 7
                next_index = line[start_index:].find("b\'\\x")
                if next_index >=0:
                        edit_index = start_index + next_index
                else:
                        edit_index = next_index
        #        output_line = output_line + line[start_index:edit_index]

        #        print("Skipping: ", line[edit_index:edit_index+7])

        #        start_index = edit_index + 7
        #        edit_index = line[start_index:].find("b\'\\x")
        #        print("Hey there:", edit_index)

        if (start_index > 0):
                output_line = output_line + bytes(line[start_index:].encode('utf-8'))

        print("OUTPUT: ", str(output_line))
        pgm_file.write(output_line)

