# Gotta love how many ways there are to print things in all the versions of Python
# I just want to print an unsigned integer in hex (like those annoying cFS error codes, for example)

def cfs_error_convert (number):
    if number < 0:
        return number + (2**32)
    else:
        return number

negative = -905969663

print("Why won't this work? %X" % negative)

print("What about this? %s" % hex(negative))

print("How about: %X" % cfs_error_convert(negative))
print("Works on 0? %X" % cfs_error_convert(0))
print("Normal numbers? %X" % cfs_error_convert(16))
