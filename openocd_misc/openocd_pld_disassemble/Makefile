
default:
	gcc -o test_pld test_pld.c

# Need to source the arm compiler? Or somethhing?
gen:
	arm-none-eabi-gcc -specs=Xilinx.spec -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -o generated generate_example.c
	arm-none-eabi-objdump -d generated > assembly.txt

clean:
	rm -f test_pld
	rm -f generated
	rm -f assembly.txt
