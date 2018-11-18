import sys

# Read some integer values from an .ini file

if len(sys.argv) < 2:
	print("Usaged: python3 read_binary.py <file.pgm>")
	exit()

print("Opening: ", sys.argv[1])

bin_file = open(sys.argv[1], "rb")

while True:
	char = bin_file.read(1)
	#if char == b'':
	#	break

	try:
		#con_char = str(c)
		con_char = char.decode('utf-8')
		print("Word: ", con_char)
		if con_char == '':
			break

	except UnicodeDecodeError: # UnicodeEncodeError:
		con_char = str(char)
		print("Not Word: ", con_char)

print("Ya done")
