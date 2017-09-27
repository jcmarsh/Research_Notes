
def parse_test(test_str):
    print("Test string: " + test_str)
    command = test_str.strip('>').split(',') # TODO: Start here
    print(command)
    registers = []
    for i in range (1, len(command)):
        if '>' in command[i]:
            pass
        else:
            reg = command[i].strip()
            # Strip the start of the register designator
            if reg[:1] == '{' or reg[:1] == '[':
                reg = reg[1:]
            if reg[-1:] == '!':
                reg = reg[:-1]
                # Strip the end of the register designator
            if reg[-1:] == ']' or reg[-1:] == '}':
                reg = reg[:-1]
                registers.append(reg)
                break

            registers.append(reg)

    print("Registers: " + str(registers))

parse_test("0x00101614 0x979ff103 LDRLS r15, [r15, r3, LSL #0x2]")
parse_test("0x0010152c 0xe5d20000 LDRB r0, [r2]")
parse_test('0x0010152c\t0xe5d20000\tLDRB r0, [r2]\r\n\r>')
