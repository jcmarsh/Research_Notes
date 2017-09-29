import ctypes

print "You should get this working."

ctypes.cdll.LoadLibrary("libarmparse.so")
libarm = ctypes.CDLL("libarmparse.so")

result = ctypes.c_char_p("This should be replaced by the desired registers.")
x = libarm.parse_line(result, "0x00100738 0xe51b2008      STR r2, [r11, #-0x8]\n")

print result.value

