# Using the run log and inject logs to match injected failures to detected / recovered.

import sys
import re

if len(sys.argv) != 2:
    print "Usage: match_errors.py <run_log.txt>"
    exit()

print "Opening: ", sys.argv[1]
run_log = open(sys.argv[1], "r")

pids_replaced = []

# Parse run log for pids
# Example file (I- from injector, V- from voter):
# I  Signal 9 on 27698 (Mapper)
# V  VoterD(Mapper) Restart handler called, 0 late
# V  VoterD(Mapper) Caught Exec / Control loop error: 0 - <27698>
# V  Load PID 27724 exited on its own. <-- Unrelated, load component
# V  Initializing controller Load
# I  Signal 9 on 27704 (ArtPot)
# V  VoterM(ArtPot) CFE or ExecFault detected: 1 - <27704>
# V  VoterM(ArtPot) is starting a replica.
# *Should* be simple: "Signal __ on pid" results in "Caught ____: X - <pid>"
# State Machine: (start) --Signal_X_on_pid--> (error to recover) --Caught_<pid>--> (start)
state = 0
injected_pid = 0
injected_count = 0
correct = 0
false_positive = 0
not_sure = 0
for line in run_log:
    if 0 == state:
        if "Signal" in line:
            injected_count = injected_count + 1
            ints_found = map(int, re.findall(r'\d+', line))
            # print 'map: ', ints_found
            injected_pid = ints_found[1]
            state = 1
        if '<' in line and '>' in line:
            # print 'False positive detected: ', line
            false_positive = false_positive + 1

    elif 1 == state:
        sta_delim = line.find('<')
	end_delim = line.find('>')

        if "Signal" in line:
            injected_count = injected_count + 1
            not_sure = not_sure + 1

        if sta_delim < 0 or end_delim < 0:
            pass
        else:
            try:
                value = int(line[sta_delim + 1:end_delim])
                # print 'recover pid:', value
                if value == injected_pid:
                    # print 'Recovered error injected in ', value
                    correct = correct + 1
                else:
                    print 'ERROR ', value, ' doesn\'t match injected ', injected_pid
                    false_positive = false_positive + 1
                state = 0
            except ValueError:
                print 'Failed to find <int> in ', line

print 'Injected count: ', injected_count
print 'Handled correctly: ', correct
print 'False positive: ', false_positive
print 'Not Sure: ', not_sure
if 1 == state:
    print 'Final injected not handled: 1'

# print pid_count, len(pids_injected), len(pids_replaced)

#win_count = 0;
#i_inj = 0
#for i_rep in range(0, pid_count):
    # print 'injected: ', pids_injected[i_inj], ' -> ', pids_replaced[i_rep], ' :result', pids_injected[i_inj] == pids_replaced[i_rep]
#    if pids_injected[i_inj] == pids_replaced[i_rep]:
#        win_count = win_count + 1
#        i_inj = i_inj + 1

#print 'Matches: ', win_count, '\tFailures: ', pid_count - win_count
