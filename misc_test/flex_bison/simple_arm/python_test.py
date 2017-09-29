import ctypes

print "You should get this working."

ctypes.cdll.LoadLibrary("libarmparse.so")
libarm = ctypes.CDLL("libarmparse.so")

result = ctypes.c_char_p("This should be replaced by the desired registers.")

# Test code
x = libarm.parse_line(result, "0x00100738 0xe51b2008      STR r2, [r11, #-0x8]\n")
print ("Ret %d, Value %s" % (x, result.value))
assert x == 0
assert result.value == "r11, #-0x8"

x = libarm.parse_line(result, "0x00100738 0xe51b2008      BLARF r2, r11\n")
print ("Ret %d, Value %s" % (x, result.value))
assert x == 0 # recognized command, but no registers to report
assert result.value == "none"

x = libarm.parse_line(result, "0x00100738 0xe51b2008      XSDB r2, [r11, #-0x8]\n")
print ("Ret %d, Value %s" % (x, result.value))
assert x == 1 # command is not recognized
assert result.value == "none"

x = libarm.parse_line(result, "0x00100738 0xe51b2008      STMDB r13!, {r0, r1, r2, r3}\n")
print ("Ret %d, Value %s" % (x, result.value))
assert x == 0
assert result.value == "r13, r0, r1, r2, r3"


