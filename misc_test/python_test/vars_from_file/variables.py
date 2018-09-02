import ConfigParser
import sys

# Read some integer values from an .ini file

if len(sys.argv) < 1:
    print "Usaged: python variables.py <file.ini>"
    exit()

print "Opening: ", sys.argv[1]

var_file = sys.argv[1]
my_config = ConfigParser.ConfigParser()
my_config.readfp(open(var_file))

inject_cycles=int(my_config.get("target",  "inject_cycles"))
inject_l2_set=int(my_config.get("target",  "inject_l2_set"))
inject_offset=int(my_config.get("target",  "inject_offset"))
inject_way=int(my_config.get("target",  "inject_way"))

print "Word", inject_cycles, inject_l2_set, inject_offset, inject_way
