/* Startup Script */
/* Copied from old plumber work. */
/* Test for if it would be possible to transition drseus to using flex/bison */

%{
#include <stdio.h>
#include <string.h>
#include "arm.h"

extern FILE *yyin;
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

int yylex();
int yyerror(char *s);

int l_count = 0;

char registers[128] = {0};
 %}

%union {
  int ival;
  float fval;
  char sval[128];
}

%token EOL

 // Shift Instructions (A4.4.2)
%token C_ASR
%token C_LSL
%token C_LSR
%token C_ROR
%token C_RRX

 // Load/Store Instructions (A4.6)
 // Handling modifiers separtely
%token C_LDR
%token C_STR

 // Load/Store Multiple Instructions (A4.7)
%token C_LDM
%token C_POP
%token C_PUSH
%token C_STM

 // PreLoad Data (ignore PLDW which indicates likely write).
 // Treat like a load: assume that data is loaded into cache
%token C_PLD

 // Values
%token <sval> ADDRESS
%token <sval> S_ADDRESS
 //%token <sval> REGISTER
%token <sval> REGISTER
%token <sval> REG_C
%token <sval> REG_P
%token <sval> OFFSET
%token <sval> FOOBAR
%token <sval> OPCODE
%token <sval> IMMEDIATE
%type<sval> dest_reg
%type<sval> s_reg
%type<sval> s_reg_and_off
%type<sval> reg_and_off
%type<sval> rec_reg
%type<sval> store_single

%token UND
%token NOP
%token DONTCARE
%token UNKNOWN

 // Formatting
%token LIST_S
%token LIST_E
%token SQUARE_S
%token SQUARE_E
%token COMMA
%token UPDATE

%%
line:
| line ADDRESS ADDRESS command EOL { } //printf("Is that a line? %d\n", ++l_count);}
| line EOL
;

command:
dont_care
| load_store
| UND
| NOP
;

dont_care:
DONTCARE ADDRESS
|DONTCARE S_ADDRESS
| DONTCARE REGISTER
| DONTCARE REGISTER COMMA REGISTER
| DONTCARE REGISTER COMMA REGISTER COMMA ignore_shift
| DONTCARE REGISTER COMMA OFFSET
| DONTCARE REGISTER COMMA REGISTER COMMA REGISTER
| DONTCARE REGISTER COMMA REGISTER COMMA REGISTER COMMA OFFSET
| DONTCARE REGISTER COMMA REGISTER COMMA REGISTER COMMA REGISTER
| DONTCARE REGISTER COMMA REGISTER COMMA REGISTER COMMA ignore_shift
| DONTCARE REGISTER COMMA REGISTER COMMA OFFSET
| DONTCARE REG_P COMMA REG_C COMMA s_reg COMMA LIST_S IMMEDIATE LIST_E
//STC p11, c8, [r13, #-16]!\r\n")
| DONTCARE REG_P COMMA REG_C COMMA s_reg_and_off
| DONTCARE REG_P COMMA REG_C COMMA s_reg_and_off UPDATE
//LDC p11, c8, [r13], #40
| DONTCARE REG_P COMMA REG_C COMMA s_reg COMMA FOOBAR
| DONTCARE REGISTER COMMA REGISTER COMMA s_reg COMMA LIST_S IMMEDIATE LIST_E
//CDP p11, 0x0b, c0, c0, c0, 0x03\n")
| DONTCARE REG_P COMMA OPCODE COMMA REG_C COMMA REG_C COMMA REG_C COMMA OPCODE
//MCR p15, 0x00, r0, c7, c5, 0x06\n")
| DONTCARE REG_P COMMA OPCODE COMMA REGISTER COMMA REG_C COMMA REG_C COMMA OPCODE
//MCRR p11, 1, r2, r3, c0
| DONTCARE REG_P COMMA IMMEDIATE COMMA REGISTER COMMA REGISTER COMMA REG_C
;

load_store:
load_predict
| load_single
| load_multiple
| store_single
| store_multiple
;

dest_reg:
REGISTER COMMA { strcpy($$, $1); }
;

ignore_shift:
C_ASR OFFSET
| C_LSL OFFSET
| C_LSR OFFSET
| C_ROR OFFSET
| C_RRX // TODO: Should this have offset? What about register version?
| C_ASR REGISTER
| C_LSL REGISTER
| C_LSR REGISTER
| C_ROR REGISTER
// | C_RRX REGISTER
;

//reg_and_reg:
//  REGISTER COMMA REGISTER
//;

reg_and_off:
REGISTER COMMA OFFSET { sprintf($$, "%s,%s", $1, $3); }
| REGISTER COMMA FOOBAR { sprintf($$, "%s,%s", $1, $3); }
;

s_reg_and_off:
SQUARE_S reg_and_off SQUARE_E { strcpy($$, $2); }
;

s_reg:
SQUARE_S REGISTER SQUARE_E { strcpy($$, $2); }
;

rec_reg:
REGISTER { strcpy($$, $1); }
| rec_reg COMMA REGISTER { sprintf($$, "%s,%s", $1, $3); }
;

load_predict:
C_PLD SQUARE_S REGISTER SQUARE_E { printf("PLD, args: %s\n", $3);
  sprintf(registers, "%s", $3); }
;

load_single:
C_LDR dest_reg s_reg { printf("LDR. dest: %s args: %s\n", $2, $3);
  sprintf(registers, "%s", $3); }
| C_LDR dest_reg s_reg_and_off { printf("LDR. dest: %s args: %s\n", $2, $3);
  sprintf(registers, "%s", $3); }
| C_LDR dest_reg s_reg_and_off UPDATE { printf("LDR. dest: %s args: %s\n", $2, $3);
  sprintf(registers, "%s", $3); }
// This is a post-index command (LDR r0, [r1], #0x4), so ignore the offset
| C_LDR dest_reg s_reg COMMA OFFSET { printf("LDR. dest: %s args: %s %s\n", $2, $3, $5);
  sprintf(registers, "%s", $3); }
// 0x00101e5a 0x461a3004 LDRMI r3, [r10], -r4 <- post-index
| C_LDR dest_reg s_reg COMMA REGISTER { printf("LDR. dest: %s args: %s %s\n", $2, $3, $5);
  sprintf(registers, "%s", $3); }
// 0x001078c0 0xe799300a LDR r3, [r9, r11]
| C_LDR dest_reg SQUARE_S dest_reg REGISTER SQUARE_E {
  printf("LDR with register offset: %s args %s %s\n", $2, $4, $5);
  sprintf(registers, "%s,%s", $4, $5); }
| C_LDR dest_reg SQUARE_S dest_reg dest_reg C_LSL OFFSET SQUARE_E {
  printf("LDR with a LSL. dest: %s args: %s %s %s\n", $2, $4, $5, $7);
  sprintf(registers, "%s,%s,LSL %s", $4, $5, $7); }
// 0x0010b6e4 0x47703801 LDRMIB r3, [r0, -r1, LSL #0x10]!
| C_LDR dest_reg SQUARE_S dest_reg dest_reg C_LSL OFFSET SQUARE_E UPDATE {
  printf("LDR with a LSL. dest: %s args: %s %s %s\n", $2, $4, $5, $7);
  sprintf(registers, "%s,%s,LSL %s", $4, $5, $7); }
// 0x0010b6e0 0x47702000 LDRMIB r2, [r0, -r0]!\n", 0, "r0,-r0")
| C_LDR dest_reg SQUARE_S dest_reg REGISTER SQUARE_E UPDATE {
  printf("LDR. dest: %s args: %s %s\n", $2, $4, $5);
  sprintf(registers, "%s,%s", $4,$5); }
;

load_multiple:
C_LDM REGISTER UPDATE COMMA LIST_S rec_reg LIST_E {printf("LDM! base: %s list: %s\n", $2, $6);
  sprintf(registers, "%s,%s", $2, $6);}
| C_LDM dest_reg LIST_S rec_reg LIST_E {printf("LDM base: %s list: %s\n", $2, $4);
  sprintf(registers, "%s,%s", $2, $4);}
;

// The UPDATE (!) should not require any changes: it auto-increments the offset... after?
store_single:
C_STR dest_reg s_reg { printf("STR dest_reg: %s args %s\n", $2, $3);
  sprintf(registers, "%s",  $3);}
| C_STR dest_reg s_reg_and_off { printf("STR dest_reg: %s args %s\n", $2, $3);
  sprintf(registers, "%s",  $3);}
| C_STR dest_reg s_reg_and_off UPDATE { printf("STR! dest_reg: %s args %s\n", $2, $3);
  sprintf(registers, "%s", $3); }
// This is a post-index command (STR r0, [r1], #0x4), so ignore the offset
| C_STR dest_reg s_reg COMMA OFFSET { printf("STR. dest: %s args: %s %s\n", $2, $3, $5);
  sprintf(registers, "%s", $3); }
// 0x00101e5a 0x461a3004 STRMI r3, [r10], -r4 <- post-index
| C_STR dest_reg s_reg COMMA REGISTER { printf("STR. dest: %s args: %s %s\n", $2, $3, $5);
  sprintf(registers, "%s", $3); }
// 0x00103cc8     0xe7821001      STR r1, [r2, r1]
| C_STR dest_reg SQUARE_S REGISTER COMMA REGISTER SQUARE_E {
  printf("STR. des: %s args: %s %s\n", $2, $4, $6);
  sprintf(registers, "%s,%s", $4, $6); }
// STRNE r1, [r3, r2, LSL #0x2]\n", 0, "r3,r2,LSL #0x2")
| C_STR dest_reg SQUARE_S REGISTER COMMA REGISTER COMMA C_LSL OFFSET SQUARE_E {
  printf("STR. des: %s args: %s %s %s\n", $2, $4, $6, $9);
  sprintf(registers, "%s,%s,LSL %s", $4, $6, $9); }
| C_STR dest_reg SQUARE_S REGISTER COMMA REGISTER COMMA C_LSL OFFSET SQUARE_E UPDATE {
  printf("STR. des: %s args: %s %s %s\n", $2, $4, $6, $9);
  sprintf(registers, "%s,%s,LSL %s", $4, $6, $9); }

;

store_multiple:
C_STM REGISTER UPDATE COMMA LIST_S rec_reg LIST_E { printf("STM! base: %s list: %s\n", $2, $6);
  sprintf(registers, "%s,%s", $2, $6); }
| C_STM dest_reg LIST_S rec_reg LIST_E { printf("STM base: %s list: %s\n", $2, $4);
    sprintf(registers, "%s,%s", $2, $4); }
;

%%
int parse_line(char result[128], char *parse) {
  YY_BUFFER_STATE state;
  strcpy(registers, "none");

  state = yy_scan_string(parse);
  int retval = yyparse();
  yy_delete_buffer(state);

  strcpy(result, registers);

  if (retval != 0) {
    printf("Parsing failure on line: %s\n", parse);
  }
  return retval;
}


int main(int argc, char **argv) {  
  if (argc > 1) {
    if (!(yyin = fopen(argv[1], "r"))) {
      perror(argv[1]);
      return(1);
    }

    yyparse();
  } else {
    char result[128] = {0};
    parse_line(result, "0x00100608	0xe50b3014	STR r3, [r11, #-0x14]\n");
    printf("Result: %s\n", result);
  }

  return 0;
}


int yyerror(char *s) {
  fprintf(stderr, "error: %s\n", s);
  return 0;
}
