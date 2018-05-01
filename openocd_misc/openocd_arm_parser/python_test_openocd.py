import ctypes

def test_line(msg, input_line, out_val, registers):
    x = libarm.parse_line(result, input_line)
    print ("%s: Ret %d, Value %s" % (msg, x, result.value))
    assert x == out_val
    assert result.value == registers

ctypes.cdll.LoadLibrary("libarmparse.so")
libarm = ctypes.CDLL("libarmparse.so")

result = ctypes.create_string_buffer(128)

# Test code

# Commands that are supported, but do not access memory
test_line("BLARF", "0x00100738 0xe51b2008      BLARF r2, r11\n", 0, "none")

test_line("MOV", "0x00100738 0xe51b2008      MOV r2, r11, LSR #0x1f\n", 0, "none")

test_line("CLZ", "0x00100738 0xe51b2008      CLZ r3, r0\n", 0, "none")

test_line("ADDNE", "0x00100738 0xe51b2008      ADDNE r2, r11, r3, LSL #0x1\n", 0, "none")

test_line("MUL", "0x00100738 0xe51b2008      MUL r2, r11, r3\n", 0, "none")

test_line("STCEQ", "0x00101dcc 0x0c00f06f      STCEQ p0, c15, [r0], {47}\n", 0, "none")

test_line("STCEQ", "0x00100738 0xe51b2008      STCEQ r2, r11, [r3], {47}\n", 0, "none")

test_line("MOV", "0x00100738 0xe51b2008      MOV r2, r3, ASR #0x6\n", 0, "none")

# Co-processor commands.
test_line("CDP", "0x001010a8 0xeeb00b60      CDP p11, 0x0b, c0, c0, c0, 0x03\n", 0, "none")

test_line("CDP", "0x00101674 0xee306b47 CDP p11, 0x03, c6, c0, c7, 0x02\n", 0, "none")

test_line("MCR", "0x00100f4c       0xee070a90      MCR p10, 0x00, r0, c7, c0, 0x04\n", 0, "none")

test_line("MRC", "0x0010139c       0xee173a90      MRC p10, 0x00, r3, c7, c0, 0x04\n", 0, "none")

test_line("MRRC", "0x00101458     0xec532b10      MRRC p11, 1, r2, r3, c0\n", 0, "none")

test_line("MCRR", "0x001021cc     0xec432b10      MCRR p11, 1, r2, r3, c0\n", 0, "none")

test_line("MCRRLELE", "0x00101840     0xdc410b15      MCRRLELE p11, 1, r0, r1, c5\n", 0, "none")

test_line("UNDEFINED", "0x00101684     0xe7ea3a53      UNDEFINED\n", 0, "none")

# Unsupported command
test_line("XSDB", "0x00100738 0xe51b2008      XSDB r2, [r11, #-0x8]\n", 1, "none")

# The following should all be successful
test_line("STMDB", "0x00100738 0xe51b2008      STMDB r13!, {r0, r1, r2, r3}\n", 0, "r13,r0,r1,r2,r3")

test_line("STR", "0x00100738 0xe51b2008      STR r2, [r11, #-0x8]\n", 0, "r11,#-0x8")

test_line("LDR", "0x00100644 0xe51b2008 LDR r2, [r11, #-0x8]\n", 0, "r11,#-0x8")

test_line("LDR", "0x001078c0 0xe799300a LDR r3, [r9, r11]\n", 0, "r9,r11")

test_line("LDRB", "0x0010152c 0xe5d20000 LDRB r0, [r2]\n", 0, "r2")

test_line("LDRLS", "0x00101618 0x979ff103 LDRLS r15, [r15, r3, LSL #0x2]\n", 0, "r15,r3,LSL #0x2")

test_line("LDM", "0x001013b0 0xe8b9000f LDM r9!, {r0, r1, r2, r3}\n", 0, "r9,r0,r1,r2,r3")

test_line("STRB", "0x001013d4 0xe5ca9000 STRB r9, [r10]\n", 0, "r10")

# STR... sometimes offsets are in registers
test_line("STR", "0x00103cc8     0xe7821001      STR r1, [r2, r1]\r\n", 0, "r2,r1")

# Post-index doesn't care about the offset
test_line("LDRMI", "0x00101e5a 0x461a3004 LDRMI r3, [r10], -r4\n", 0, "r10")

test_line("LDMIB", "0x001023ec 0xe9d14770 LDMIB r1, {r4, r5, r6, r8, r9, r10, r14}\n", 0, "r1,r4,r5,r6,r8,r9,r10,r14")

# Use the infinite loop to check for memory leaks.
# while(True):

test_line("STMDB", "0x00101398 0xe92d47f0 STMDB r13!, {r4, r5, r6, r7, r8, r9, r10, r14}\n", 0, "r13,r4,r5,r6,r7,r8,r9,r10,r14")

test_line("SUB", "0x00101398      0xe24dd014      SUB r13, r13, #0x14\r\n", 0, "none")
test_line("STC", "0x00100de0     0xed2d8b04      STC p11, c8, [r13, #-16]!\r\n", 0, "none")
test_line("LDC", "0x00104b24 0xecbd8b0a LDC p11, c8, [r13], #40\n", 0, "none") # Post index update! But it's coprocessor related anyways, so no DONTCARE

# Apparently some offsets for shifts (like LSL) can be a register
test_line("MOV", "0x00103c14     0xe1a0e31e      MOV r14, r14, LSL r3\r\n", 0, "none")

test_line("MLA", "0x00104534     0xe0211c92      MLA r1, r2, r12, r1\r\n", 0, "none")

test_line("UMULL", "0x00104538     0xe0832293      UMULL r3, r2, r3, r2\r\n", 0, "none")


# New batch of failures:
test_line("NOP", "0x001008ac 0xe1a00000 NOP\n", 0, "none")

# SVC is a supervisor call
test_line("SVC", "0x0010467e 0x0f04f014 SVC 0x04f014\n", 0, "none")

test_line("RFEDA", "0x0010b6d0 0xf810b132 RFEDA r0\n", 0, "none")

test_line("LDRMIB", "0x0010b6e0 0x47702000 LDRMIB r2, [r0, -r0]!\n", 0, "r0,-r0")

test_line("LDRMIB", "0x0010b6e4 0x47703801 LDRMIB r3, [r0, -r1, LSL #0x10]!\n", 0, "r0,-r1,LSL #0x10")


# (un)signed expand byte
test_line("UXTB", "0x00109144 0xe6ff2073 UXTB r2, r3\n", 0, "none")
test_line("UXTBNE", "0x0010bf28 0x16ef1071 UXTBNE r1, r1\n", 0, "none")
test_line("SXTB", "0x00109144 0xe6ff2073 SXTB r2, r3\n", 0, "none")


test_line("STRNE", "0x0010c000 0x17831102 STRNE r1, [r3, r2, LSL #0x2]\n", 0, "r3,r2,LSL #0x2")
test_line("STR", "0x0010bf88 0xe7821104 STR r1, [r2, r4, LSL #0x2]\n", 0, "r2,r4,LSL #0x2")
                                                                      
# PLD... not sure how to handle
test_line("PLD", "0x00101420 0xf5d4f000 PLD [r4]\n", 0, "r4")
