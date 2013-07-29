import sys 
import ConfigParser

results_name = "results.txt"
#folder_name = "uturn_convoy_test"
folder_name = "door_test"
# Parameters that have multiple values
#maps = ["./bitmaps/uturn_18.png",
#        "./bitmaps/uturn_36.png",
#        "./bitmaps/uturn_54.png",
#        "./bitmaps/uturn_72.png",
#        "./bitmaps/uturn_90.png"]
#maps = ["./bitmaps/grid_18.png",
#        "./bitmaps/grid_36.png",
#        "./bitmaps/grid_54.png",
#        "./bitmaps/grid_72.png",
#        "./bitmaps/grid_90.png"]
maps = ["./bitmaps/door.png"]

speedups = ["1"] #, "2", "4", "8"]
# Manager that will coordinate the controllers
manager = "linked_manager"
# Controllers are the same in all tests
controllers = ["leader", "follower", "follower", "door"]
# Names should be the same length as Controllers, 1 to 1
names = ["hank", "frank", "samantha", "wall"]

# The default parameters
config = ConfigParser.RawConfigParser()

config.add_section("files")
config.set("files", "cfg", "./configs/multi_comp_door.cfg")
config.set("files", "world", "./worlds/multi_simple_door.world")
config.set("files", "map", "SET_THIS") # Set with an element from maps

config.add_section("controllers")
config.set("controllers", "manager", manager)
config.set("controllers", "num", str(len(controllers)))
for i in range(len(controllers)):
    config.set("controllers", "cont" + str(i), controllers[i])
    config.set("controllers", "name" + str(i), names[i])

config.add_section("experiment")
config.set("experiment", "runs", "10")
config.set("experiment", "timeout", "180")

config.add_section("worldfile")
config.set("worldfile", "speedup", "SET_THIS") # Set with an element from speedups


# Make all of the .ini files
count = 0
for i in range(0, len(speedups)):
    config.set("worldfile", "speedup", speedups[i])
    for j in range(0, len(maps)):
        config.set("files", "map", maps[j])
        new_file = open("conf_" + str(count) + ".ini", "w")
        config.write(new_file)
        count = count + 1


# Make a file "run_set.sh"
bash_script = open("run_set.sh", "w")
bash_script.write("#!/bin/bash\n\n")
bash_script.write("# Generated by script_gen.py\n\n")

count = 0
for i in range(0, len(speedups)):
    for j in range(0, len(maps)):
        bash_script.write("echo \"" + str(count) + " Experiment: ############################\"\n")
        bash_script.write("python overlord.py ./experiments/" + folder_name + "/conf_" + str(count) + ".ini " + results_name + "\n\n")
        count = count + 1



