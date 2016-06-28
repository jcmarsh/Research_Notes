# Using the run log and inject logs to match injected failures to detected / recovered.

import sys

if len(sys.argv) != 3:
    print "Usage: match_errors.py <run_file.txt> <inject_log.txt>"
    exit()

print "Opening: ", sys.argv[1]
run_log = open(sys.argv[1], "r")

print "Opening: ", sys.argv[2]
inj_log = open(sys.argv[2], "r")

pids_replaced = []

# Parse run log for pids of replaced replicas
# example line: VoterM(Filter) (SC) CFE or ExecFault detected: 0 - <5989>
for line in run_log:
    if line[0:5] == 'Voter':
        sta_delim = line.find('<')
	end_delim = line.find('>')
        
        if sta_delim < 0 or end_delim < 0:
            pass
        else:
            try:
                value = int(line[sta_delim + 1:end_delim])
            except ValueError:
                print 'Failed to find <int> in ', line

            pids_replaced.append(value)

            # print 'line: ', line
            # print '\t pid:', value

# Parse run log for pids of replaced replicas
# example line: Signal 9 on 27355 (ArtPot)
pids_injected = []

for line in inj_log:
    if line[0:6] == 'Signal':
        values = line.split(' ')
        value = values[3]

        try:
            pid = int(value)
        except ValueError:
            print 'Failed to find pid in ', line

        pids_injected.append(pid)

        # print 'line: ', line
        # print '\t pid:', pid

pid_count = 0
if len(pids_injected) < len(pids_replaced):
    pid_count = len(pids_injected)
else:
    pid_count = len(pids_replaced)

# print pid_count, len(pids_injected), len(pids_replaced)

win_count = 0;
i_inj = 0
for i_rep in range(0, pid_count):
    print 'injected: ', pids_injected[i_inj], ' -> ', pids_replaced[i_rep], ' :result', pids_injected[i_inj] == pids_replaced[i_rep]
    if pids_injected[i_inj] == pids_replaced[i_rep]:
        win_count = win_count + 1
        i_inj = i_inj + 1

print 'Matches: ', win_count, '\tFailures: ', pid_count - win_count
