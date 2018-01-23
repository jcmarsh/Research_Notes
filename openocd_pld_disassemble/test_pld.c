#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct arm_instruction {
  //enum arm_instruction_type type;
	char text[128];
	uint32_t opcode;

	/* return value ... Thumb-2 sizes vary */
	unsigned instruction_size;

  /*
	union {
		struct arm_b_bl_bx_blx_instr b_bl_bx_blx;
		struct arm_data_proc_instr data_proc;
		struct arm_load_store_instr load_store;
		struct arm_load_store_multiple_instr load_store_multiple;
	} info;
  */
};

static int evaluate_pld(uint32_t opcode,
			uint32_t address, struct arm_instruction *instruction)
{
	// only called if opcode starts with 11110
	/* PLD */
	// if ((opcode & 0x0d70f000) == 0x0550f000) {
	if ((opcode & 0x0d50f000) == 0x0550f000) { // Should allow PLDW commands as well
		uint8_t Rn;
		uint8_t U;
		unsigned offset;
		// instruction->type = ARM_PLD;
		Rn = (opcode & 0xf0000) >> 16;
		U = (opcode & 0x00800000) >> 23;
		if (Rn == 0xf) { // literal
			offset = opcode & 0x0fff;
			snprintf(instruction->text, 128,
				 "0x%8.8X\t0x%8.8X\tPLD %s%d",
				 address, opcode, U ? "" : "-", offset);
		} else {
			uint8_t I, R;
                        //0xf5d9f000; U = 1, I = 0, R = 1 1111 0101 1101 1001 1111 0000 0000 0000
			I = (opcode & 0x02000000) >> 25;
                        R = (opcode & 0x00400000) >> 22;

			if (I) { // register PLD{W} [<Rn>,+/-<Rm>{, <shift>}]
                          printf("Reg baby!\n");
				offset = (opcode & 0x0F80) >> 7;
				uint8_t Rm;
				Rm = opcode & 0xf;

				if (offset == 0) {
					// No shift
					snprintf(instruction->text, 128,
						 "0x%8.8X\t0x%8.8X\tPLD%s [r%d, %sr%d]",
						 address, opcode, R ? "" : "W", Rn, U ? "" : "-", Rm);

				} else {
					uint8_t shift;
					shift = (opcode & 0x60) >> 5;

					if (shift == 0x0) { // LSL
						snprintf(instruction->text, 128,
							 "0x%8.8X\t0x%8.8X\tPLD%s [r%d, %sr%d, LSL #0x%x)",
							 address, opcode, R ? "" : "W", Rn, U ? "" : "-", Rm, offset);
					} else if (shift == 0x1) { // LSR
						snprintf(instruction->text, 128,
							 "0x%8.8X\t0x%8.8X\tPLD%s [r%d, %sr%d, LSR #0x%x)",
							 address, opcode, R ? "" : "W", Rn, U ? "" : "-", Rm, offset);
					} else if (shift == 0x2) { // ASR
						snprintf(instruction->text, 128,
							 "0x%8.8X\t0x%8.8X\tPLD%s [r%d, %sr%d, ASR #0x%x)",
							 address, opcode, R ? "" : "W", Rn, U ? "" : "-", Rm, offset);
					} else if (shift == 0x3) { // ROR
						snprintf(instruction->text, 128,
							 "0x%8.8X\t0x%8.8X\tPLD%s [r%d, %sr%d, ROR #0x%x)",
							 address, opcode, R ? "" : "W", Rn, U ? "" : "-", Rm, offset);
					}
				}
			} else { // immediate PLD{W} [<Rn>, #+/-<imm12>]
                          printf("It's happenin now!\n");
                                                  //0xf5d9f000; U = 1, I = 0, R = 1 1111 0101 1101 1001 1111 0000 0000 0000
				offset = opcode & 0x0fff;
				if (offset == 0) {
					snprintf(instruction->text, 128,
						 "0x%8.8X\t0x%8.8X\tPLD%s [r%d]",
						 address, opcode, R ? "" : "W", Rn);
				} else {
					snprintf(instruction->text, 128,
						 "0x%8.8X\t0x%8.8X\tPLD%s [r%d, #%s%d]",
						 address, opcode, R ? "" : "W", Rn, U ? "" : "-", offset);
				}
			}
		}
		return 1;
	}
	/* DSB */
	if ((opcode & 0x07f000f0) == 0x05700040) {
          //instruction->type = ARM_DSB;

		char *opt;
		switch (opcode & 0x0000000f) {
		case 0xf:
			opt = "SY";
			break;
		case 0xe:
			opt = "ST";
			break;
		case 0xb:
			opt = "ISH";
			break;
		case 0xa:
			opt = "ISHST";
			break;
		case 0x7:
			opt = "NSH";
			break;
		case 0x6:
			opt = "NSHST";
			break;
		case 0x3:
			opt = "OSH";
			break;
		case 0x2:
			opt = "OSHST";
			break;
		default:
			opt = "UNK";
		}

		snprintf(instruction->text,
				128,
				"0x%8.8X\t0x%8.8X\tDSB %s",
				address, opcode, opt);

		return 1;
	}
	/* ISB */
	if ((opcode & 0x07f000f0) == 0x05700060) {
          //instruction->type = ARM_ISB;

		snprintf(instruction->text,
				128,
				"0x%8.8X\t0x%8.8X\tISB %s",
				address, opcode,
				((opcode & 0x0000000f) == 0xf) ? "SY" : "UNK");

		return 1;
	}
        printf("UNKNOWN\n");
        return 1;
}

void main() {

  // Sample pld instructions: 0x00101420 0xf5d4f000 PLD ...TODO...
  uint32_t address = 0x00101420;
  uint32_t opcode =  0xf5d4f000;
  struct arm_instruction inst;

  evaluate_pld(opcode, address, &inst);
  printf("output text: %s\n", inst.text);

  address = 0x001007e8;
  opcode = 0xf5d9f000;

  evaluate_pld(opcode, address, &inst);
  printf("output text: %s\n", inst.text);

  address = 0x00100800;
  opcode = 0xf5d7f000;

  evaluate_pld(opcode, address, &inst);
  printf("output text: %s\n", inst.text);
}
