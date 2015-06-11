import math
import random

maximum = .31831
average = .159163

def getNormal():
    #x = random.random()
    #x = (x * math.pi * 2) - math.pi
    #y = (1 + math.cos(x)) / (2 * math.pi)

    u_0 = random.random()
    u_1 = random.random()

    z_0 = math.sqrt(-2 * math.log(u_0)) * math.cos(2 * math.pi * u_1)
    z_1 = math.sqrt(-2 * math.log(u_0)) * math.sin(2 * math.pi * u_1)

    print "%f\t%f" % (z_0, z_1)
    return z_0, z_1

sum = 0
bins = [0 for x in range(10)]

step = (math.pi * 2) / len(bins)

def run_set(trials):
    sum = 0
    for i in range(trials):
        getNormal()
        #    return sum / trials
#    if x > max:
#        max = x
#        i = int(x / step)
#        bins[i] = bins[i] + 1
        #print "Value %f is placed into bin %d" % (x, i)
        
#    for i in range(len(bins)):
        #print "\tBin %d: %d" % (i, bins[i])
#        print "%f\t%f" % (i * step - (maximum / 2), bins[i])


run_set(10000)


#for i in range(100):
#    new_sum = run_set(1000000)
#    print "Run %d: %f" % (i, new_sum)
#    sum = sum + new_sum
