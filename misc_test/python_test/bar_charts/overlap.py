# libraries
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rc

benchmark = ["Matrix Mult", "LZO", "Basicmath", "Bitcount", "Qsort", "Susan", "ADPCM", "CRC32", "FFT", "GSM"]
resident = [1.114, 23.525, 4.171, 0.056, 19.79, 2.952, 7.852, 12.648, 11.527, 0.771]
critical = [0.19, 9.804, 0.252, 0.046, 9.852, 1.573, 0.033, 0.206, 3.438, 0.319]
dormant = [0.118, 6.118, 0.17, 0.016, 9.814, 1.505, 0.005, 0.035, 3.388, 0.271]

norm_resident = []
norm_critical = []
norm_dormant = []

barWidth = .8
r = []
label_r = []
start_pos = .1

for index in range(0, len(resident)):
	norm_resident.append(resident[index] / resident[index])
	norm_critical.append(critical[index] / resident[index])
	norm_dormant.append(dormant[index] / resident[index])
	r.append(start_pos)
	label_r.append(start_pos + .35)
	start_pos = start_pos + 1

# y-axis in bold
#rc('font', weight='bold')

# Create brown bars
plt.bar(r, norm_resident, color='#f1a340', width=barWidth)
# Create green bars (middle), on top of the firs ones
plt.bar(r, norm_critical, color='#d0d0d0', width=barWidth-.2)
# Create green bars (top)
plt.bar(r, norm_dormant, color='#998ec3', width=barWidth-.4)

# Custom X axis
plt.xticks(label_r, benchmark, rotation=90)
plt.xlabel("Benchmark")
plt.ylabel("Proportion of Resident Data")
plt.legend(["Resident", "Critical", "Dormant Critical"])

plt.tight_layout()

# Show graphic
plt.savefig('out.svg', bbox_inches='tight', pad_inches=0.2)
plt.show()
