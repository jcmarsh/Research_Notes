# read a file, assume form of <key>: <integer_value>
# print all of these pairs at the end.

import sys
import re

if len(sys.argv) != 2:
    print "Usage: key_int.py <filte.txt>"
    exit()

print "Opening: ", sys.argv[1]
log = open(sys.argv[1], "r")

pairs = {}
for line in log:
    if ':' in line:
        location = line.find(':')
        key = line[0:location]
        try:
            value = int(re.search(r'\d+', line[location:-1]).group())
            print line, ': key <', key, '> value <', value, '>'
            if key in pairs:
                pairs[key] = pairs[key] + value
            else:
                pairs[key] = value
        except AttributeError:
            pass

print pairs


            


