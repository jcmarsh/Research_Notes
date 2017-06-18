#!/usr/bin/env python

# Copied in part from the player source examples.

import math
import sys
import algs
from playerc import *
from stage_utils import *

# Create client object
client = startup(sys.argv, "run_temp.cfg")
pos, ran, gra = create_std(client)

# figure out the location of the target (from the world file) in robot coords.
target_loc = search_pose("run_temp.world", "target0")
drive_type = search_text_property("gridcar.inc", "drive")
goal = Point(target_loc[0], target_loc[1])

print "The target is at: %.2f %.2f" % (goal.x, goal.y)

old_del_x = 0
old_del_y = 0
speed = 8

while(True):
    id = client.read()

    delta, total_factors = algs.potential(pos, ran, goal)

    # Now we have del_x and del_y, which describes the vetor along which the robot should move.
    if drive_type == "omni":
        delta.x = delta.x / total_factors
        delta.y = delta.y / total_factors
        pos.set_cmd_vel(speed * delta.x, speed * delta.y, 0, 1)
    elif drive_type == "diff":
        # Should include current heading to damping sudden changes
        total_factors += 1
        delta.x += old_del_x
        delta.y += old_del_y
        delta.x = delta.x / total_factors
        delta.y = delta.y / total_factors

        gra.clear()
        gra.draw_polyline([(0, 0), (delta.x, delta.y)], 2)

        vel = speed * math.sqrt(math.pow(delta.x, 2) + math.pow(delta.y, 2))
        rot_vel = speed * math.atan2(delta.y, delta.x)
        
        pos.set_cmd_vel(vel, 0.0, rot_vel, 1)
        old_del_x = delta.x
        old_del_y = delta.y
    else:
        print("Unrecognized drive type: ", drive_type)

print("DONE!")

