import sys
import sqlite3

# Load a database file from an asm_golden run
# Generate a graph of the cache usage (total occupied), vulnerability (total that will be used again), and unsure

# Do this by stepping through each valid_line
# base time off of cycles


####-- Start Script --####

if len(sys.argv) < 1:
    print "Usage: python new_vuln_chart.py <sqlite_file>"
    exit()

#print "Opening: ", sys.argv[1]
conn = sqlite3.connect(sys.argv[1])
cur = conn.cursor()

# Set a larger cache size for the database
cur.execute("PRAGMA cache_size = 10000")

cur.execute("SELECT MIN(cycles_t) FROM ls_inst")
start_cycle = cur.fetchone()[0]

cur.execute("SELECT MAX(cycles_t) FROM ls_inst")
end_cycle = cur.fetchone()[0]

print "Start cycle: ", start_cycle, "\tEnd cycle: ", end_cycle

vuln_time_total = 0
vuln_time_ranges = []
late_time_total = 0
late_time_ranges = []
util_time_total = 0
util_time_ranges = []

cur.execute("SELECT vl_id, tag, l2_set, way, cycle_in, cycle_last_used, cycle_out FROM valid_lines")

valid_lines = cur.fetchall()

# TODO: Deal with evictions for cache utilization
# TODO: Check that the last access is a load, not a store (should have started a new chunk)
# TODO: Am I dealing with ways correctly? <- chunks are identified by tag+l2_set, so multiple l2_set entries can exist. It doesn't count them (infinite ways)

prev_l2_set = -1
prev_tag = -1

for valid_line in valid_lines:
    # For each data valid_line:
    #   Calculate the time from it's first to last access
    #   Calculate the time from when it entered to the end of the run (utilization)
    access_time = {}

    vl_id = valid_line[0]
    tag = valid_line[1]
    l2_set = valid_line[2]
    way = valid_line[3]
    cycle_in = valid_line[4]
    cycle_last_used = valid_line[5]
    cycle_out = valid_line[6]

    print "Valid Line:", valid_line

    if cycle_in == -1:
        cycle_in = start_cycle
    
    #if (prev_l2_set != l2_set) and (prev_tag != tag):
    #if cycle_last_used != cycle_out:
    if cycle_last_used != cycle_in: # No need to include these; one off uses
        vuln_time_total = vuln_time_total + (cycle_last_used - cycle_in)
        vuln_time_ranges.append((cycle_in, cycle_last_used))
        print "\tVuln amount: {} - {} = {}".format(cycle_last_used, cycle_in, cycle_last_used - cycle_in)

    if cycle_out == -1:
        cycle_out = end_cycle

    util_time_total = util_time_total + (cycle_out - cycle_in)
    util_time_ranges.append((cycle_in, cycle_out))
    print "\tUtil amount: {} - {} = {}".format(cycle_out, cycle_in, cycle_out - cycle_in)

    # TODO: Doing a better job with util will require ways / l2_set checks (if overwriting an exsing line, it shouldn't be double counted

    # TODO: How to calculate the "latent" uses? Find gaps of 10K (ish) in accesses? Do that here or in the loops below?
    # Loop through accesses to get all the big gaps, and add them as their own time ranges.
    cur.execute("SELECT cycle FROM accesses WHERE vl_id = {} ORDER BY cycle ASC".format(vl_id))
    accesses = cur.fetchall()
    prev_cycle = -1

    for access in accesses:
        cycle = access[0]

        if not prev_cycle == -1:
            if cycle - prev_cycle > 14000:
                late_time_total = late_time_total + (cycle - prev_cycle)
                late_time_ranges.append((prev_cycle, cycle))
                print "\tLate amount: {} - {} = {}".format(cycle, prev_cycle, cycle - prev_cycle)
        prev_cycle = cycle

print("All done.")
print("vuln_time: %d" % (vuln_time_total))
print("All the TIMES:")
print(vuln_time_ranges)
print("late_time: %d" % (late_time_total))
print("All the TIMES:")
print(late_time_ranges)
print("util_time: %d" % (util_time_total))
print("All the TIMES:")
print(util_time_ranges)


# alright, now it's time to make this data pretty.
granularity = 10000 * 14
total_time = start_cycle
vuln_steps = []
late_steps = []
util_steps = []

while total_time < end_cycle:
    vuln_block_count = 0
    late_block_count = 0
    util_block_count = 0

    for vuln_range in vuln_time_ranges:
        if vuln_range[0] <= total_time and vuln_range[1] > total_time:
            vuln_block_count = vuln_block_count + 1
    vuln_steps.append(vuln_block_count)

    for late_range in late_time_ranges:
        if late_range[0] <= total_time and late_range[1] > total_time:
            late_block_count = late_block_count + 1
    late_steps.append(late_block_count)

    for util_range in util_time_ranges:
        if util_range[0] <= total_time and util_range[1] > total_time:
            util_block_count = util_block_count + 1
    util_steps.append(util_block_count)

    total_time = total_time + granularity

# TODO: Need a way to reduce the weight of the final section since it may be only a portion of the cycle granularity. Could just multiply by the ratio of the last line size to granularity.

# Print as a .csv file
print("Filename: %s" % (sys.argv[1]))
print("Step size is: %d" % (granularity))
print("Not used in X cycles, Total Vulnerable Blocks, Total Utilized")
for i in range(0, len(vuln_steps)):
    print("%d, %d, %d" % (late_steps[i], vuln_steps[i], util_steps[i]))
