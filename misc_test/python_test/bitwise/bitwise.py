from time import sleep

# Returns if the conditional check was true or false. Assumes is_conditional() was true
def resolve_conditional(cpsr, sub_command):
    # cpsr is read by the step() function

    # TODO: No idea if this works how I think it should...

    # 31: N, Negative result from ALU
    N = cpsr >> 31
    # 30: Z, Zero result from ALU
    Z = (cpsr >> 30) & 0x1
    # 29: C, ALU operation carried out
    C = (cpsr >> 29) & 0x1
    # 28: V, ALU operation overflowed
    V = (cpsr >> 28) & 0x1

    if sub_command is None:
        return False
    elif 'EQ' in sub_command:
        # Z set
        return Z
    elif 'NE' in sub_command:
        # Z clear
        return not Z
    elif 'MI' in sub_command:
        # N set
        return N
    elif 'PL' in sub_command:
        # N clear
        return not N
    elif 'HI' in sub_command:
        # C set and Z clear
        return C and (not Z)
    elif 'LS' in sub_command:
        # C clear or Z set
        return (not C) or Z
    elif 'GE' in sub_command:
        # N == V
        return N == V
    elif 'LT' in sub_command:
        # N != V
        return N != V
    elif 'GT' in sub_command:
        # Z clear & N == V
        return (not Z) and (N == V)
    elif 'LE' in sub_command:
        # Z set & N != V
        return Z and (N != V)
    elif 'VS' in sub_command:
        # V set
        return V
    elif 'VC' in sub_command:
        # V clear
        return not V
    elif 'CS' in sub_command:
        # C set
        return C
    elif 'CC' in sub_command:
        # C clear
        return not C
    else:
        return False

# --------------------- START ---------------------

print("Hey, I'm testing here")

# 31: N, Negative result from ALU
# 30: Z, Zero result from ALU
# 29: C, ALU operation carried out
# 28: V, ALU operation overflowed

# Test EQ, Z set
print("# Test EQ, Z set")
sub_command = 'EQ'
cpsr = 0x4FFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0xBFFFFFFF
assert(not(resolve_conditional(cpsr, sub_command)))


# Test NE, Z clear
print("# Test NE, Z clear")
sub_command = 'NE'
cpsr = 0x0FFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0xFFFFFFFF
assert(not(resolve_conditional(cpsr, sub_command)))

# Test MI, N set
print("# Test MI, N set")
_sub_command = 'MI'
cpsr = 0x8FFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0x7FFFFFFF
assert(not(resolve_conditional(cpsr, sub_command)))

# Test PL, N clear
print("# Test PL, N clear")
sub_command = 'PL'
cpsr = 0x0FFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0xFFFFFFFF
assert(not(resolve_conditional(cpsr, sub_command)))

# Test HI, C set and Z clear
print("# Test HI, C set and Z clear")
sub_command = 'HI'
cpsr = 0x2FFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0xBFFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0x6FFFFFFF
assert(not(resolve_conditional(cpsr, sub_command)))
cpsr = 0xDFFFFFFF
assert(not(resolve_conditional(cpsr, sub_command)))

# Test LS, C clear or Z set
print("# Test LS, C clear or Z set")
sub_command = 'LS'
cpsr = 0xDFFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0x4FFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0x8FFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0x2FFFFFFF
assert(not(resolve_conditional(cpsr, sub_command)))

# Test GE, N == V
print("# Test GE, N == V")
sub_command = 'GE'
cpsr = 0x6FFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0x9FFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0x1FFFFFFF
assert(not(resolve_conditional(cpsr, sub_command)))

# Test LT, N != V
print("# Test LT, N != V")
sub_command = 'LT'
cpsr = 0x1FFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0x6FFFFFFF
assert(not(resolve_conditional(cpsr, sub_command)))
cpsr = 0x9FFFFFFF
assert(not(resolve_conditional(cpsr, sub_command)))

# Test GT, Z clear & N == V
print("# Test GT, Z clear & N == V")
sub_command = 'GT'
cpsr = 0x2FFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0x0FFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0xBFFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0x1FFFFFFF
assert(not(resolve_conditional(cpsr, sub_command)))
cpsr = 0x4FFFFFFF
assert(not(resolve_conditional(cpsr, sub_command)))

# Test LE, Z set & N != V
print("# Test LE, Z set & N != V")
sub_command = 'LE'
cpsr = 0x5FFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0x4FFFFFFF
assert(not(resolve_conditional(cpsr, sub_command)))

# Test VS, V set
print("# Test VS, V set")
sub_command = 'VS'
cpsr = 0x1FFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0xEFFFFFFF
assert(not(resolve_conditional(cpsr, sub_command)))

# Test VC, V clear
print("# Test VC, V clear")
sub_command = 'VC'
cpsr = 0xEFFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0x1FFFFFFF
assert(not(resolve_conditional(cpsr, sub_command)))

# Test CS, C set
print("# Test CS, C set")
sub_command = 'CS'
cpsr = 0x2FFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0xDFFFFFFF
assert(not(resolve_conditional(cpsr, sub_command)))

# Test CC, C clear
print("# Test CC, C clear")
sub_command = 'CC'
cpsr = 0xDFFFFFFF
assert(resolve_conditional(cpsr, sub_command))
cpsr = 0x2FFFFFFF
assert(not(resolve_conditional(cpsr, sub_command)))
