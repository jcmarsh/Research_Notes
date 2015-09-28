import sys
import numpy as np

if len(sys.argv) < 1:
	print "Usage: python run_stats.py <file_0> ... <file_n>"
	exit()

datas = []

for i in range(1, len(sys.argv)):
	print "Opening: ", sys.argv[i]
	datas.append(open(sys.argv[i], "r"))

x = []
y = []

all_vel = []
all_times = []

# stats from load
all_ped_time = []
all_ped_count = [] 

file_num = 0
for data_file in datas:
	n = 0
	total_vel = 0
        elapsed_time = 0.0

        ped_time = 0
        ped_count = 0

	x.append([])
	y.append([])
	for line in data_file:
		if line[0] == '(' and line[-2] == ')':
			try:
				values = line[1:-2]
				value = values.split(',')
				x[file_num].append(float(value[4]))
				y[file_num].append(float(value[5]))
				# what about mins?
				total_vel = total_vel + float(value[1])
                                elapsed_time = elapsed_time + float(value[3])
				n = n + 1
			except ValueError:
				print "Failed to interpret line: ", line
		elif "LOGGED MSG: Ped total:" in line:
                        try:
                                #print "PED LINE: ", line
                                value = line.split(' ')
                                ped_count = ped_count + int(value[4])
                                #print "PEDCOUNT: ", ped_count, value[4]
                                ped_time = ped_time + int(value[7][0:-3])
                                # print "PED TIME: ", ped_time, value[7]
                        except ValueError:
                                print "Failed to interpret line: ", line
                else:
			print "Poorly formatted line: ", line
	print "Count  : ", n
	all_vel.append(total_vel / n)
	file_num = file_num + 1
        all_times.append(elapsed_time)

        all_ped_count.append(ped_count)
        all_ped_time.append(ped_time)
	data_file.close()

print "All right... let's see if this makes sense:"
print "average time: ", sum(all_times) / file_num
print "standard dev: ", np.std(all_times)
print "WCET ???????: ", max(all_times)
if sum(all_ped_count) > 0:
        print "avg ped  c/t: ", sum(all_ped_count) / file_num, sum(all_ped_time) / file_num
        print "peds per sec: ", sum(all_ped_count) / (sum(all_ped_time) / 1000.0)
        print "| ", sum(all_times) / file_num, " | ", np.std(all_times), " | ", max(all_times), " | ", sum(all_ped_count) / (sum(all_ped_time) / 1000.0), " |"
else:
        print "| ", sum(all_times) / file_num, " | ", np.std(all_times), " | ", max(all_times), " | ", 0, " |"
