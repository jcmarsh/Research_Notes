/* Startup Script */
/* Copied from old plumber work. */
/* Test for if it would be possible to transition drseus to using flex/bison */

%{
#include <stdio.h>

extern FILE *yyin;

int yylex();
int yyerror(char *s);

int l_count = 0;
%}

%union {
  int ival;
  float fval;
  char *sval;
}

%token EOL

 // Branch instructsion (A4.3)
%token C_B
%token C_CBNZ
%token C_CBZ
%token C_BL
%token C_BLX
%token C_BX
%token C_BXJ
%token C_TBB
%token C_TBH

 // Conditionals
%token COND_EQ
%token COND_NE
%token COND_CS
%token COND_CC
%token COND_MI
%token COND_PL
%token COND_VS
%token COND_VC
%token COND_HI
%token COND_LS
%token COND_GE
%token COND_LT
%token COND_GT
%token COND_LE


 // Standard Data Processing (A4.4.1)
%token C_ADC
%token C_ADD
%token C_ADR
%token C_AND
%token C_BIC
%token C_CMN
%token C_CMP
%token C_EOR
%token C_MOV
%token C_MVN
%token C_ORN
%token C_ORR
%token C_RSB
%token C_RSC
%token C_SBC
%token C_SUB
%token C_TEQ
%token C_TST

 // Shift Instructions (A4.4.2)
%token C_ASR
%token C_LSL
%token C_LSR
%token C_ROR
%token C_RRX

 // Multiply Instructions (A4.4.3)
 // TODO

 // Saturating INstructions (A4.4.4)
 // TODO

 // Saturating Addition and Subtraction Instructions (A4.4.5)
 // TODO

 // Packing and Unpacking Instructions (A4.4.6)
 // TODO

 // Parallel Addition and Subtraction Instructions (A4.4.7)
 // TODO

 // Divide Instructions (A4.4.8)
 // TODO

 // Miscellaneous Data-Processing Instructions (A4.4.9)
 // TODO

 // Status Register Access Instructions (A4.5)
 // TODO

 // Banked Register Access Instructions (A4.5.1)
 // TODO

 // Load/Store Instructions (A4.6)
 // Handling modifiers separtely
%token C_LDR
%token C_STR

 // Load/Store Modifiers for Single Instructions
%token M_HALF
%token M_SIGNED
%token M_BYTE
%token M_DOUBLE
%token M_UNPRIV
%token M_EXCLU

 // Load/Store Multiple Instructions (A4.7)
%token C_LDM
%token C_POP
%token C_PUSH
%token C_STM

 // Load/Store Modifiers for Multiple Instructions
%token M_IA
%token M_DA
%token M_FD
%token M_FA
%token M_DB
%token M_IB
%token M_EA
%token M_ED

 // Values
%token <sval> ADDRESS
%token <sval> REGISTER
%token <sval> OFFSET

%token UND

 // Formatting
%token LIST_S
%token LIST_E
%token SQUARE_S
%token SQUARE_E
%token COMMA
%token UPDATE

%%
line:
| line ADDRESS ADDRESS command EOL { printf("Is that a line? %d\n", ++l_count);}
| line EOL
;

command:
  arithmetic
| branch
| load_store
| UND
;

// I don't really care about arithmetic commands
// TODO: dest_reg OFFSET is the same as reg_and_off...
arithmetic:
  C_ADD dest_reg reg_and_off
| C_ADD dest_reg reg_and_reg
| C_CMP dest_reg REGISTER
| C_CMP dest_reg OFFSET
| C_SUB dest_reg reg_and_off
| C_MOV reg_and_off
| C_MOV reg_and_reg
;

branch:
  C_B ADDRESS
| C_B cond ADDRESS
| C_BL ADDRESS
| C_BL M_UNPRIV ADDRESS
| C_BX REGISTER
;

cond:
  COND_EQ
| COND_NE
| COND_CS
| COND_CC
| COND_MI
| COND_PL
| COND_VS
| COND_VC
| COND_HI
| COND_LS
| COND_GE
| COND_LT
| COND_GT
| COND_LE
;

load_store:
  load_single
| load_multiple
| store_single
| store_multiple
;

dest_reg:
  REGISTER COMMA
;

reg_and_reg:
  REGISTER COMMA REGISTER
;

reg_and_off:
  REGISTER COMMA OFFSET
;

s_reg_and_off:
  SQUARE_S reg_and_off SQUARE_E
;

s_reg:
  SQUARE_S REGISTER SQUARE_E
;

reg_list:
  LIST_S REGISTER LIST_E
| LIST_S dest_reg REGISTER LIST_E
| LIST_S dest_reg dest_reg REGISTER LIST_E
| LIST_S dest_reg dest_reg dest_reg REGISTER LIST_E
| LIST_S dest_reg dest_reg dest_reg dest_reg REGISTER LIST_E
| LIST_S dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg REGISTER LIST_E
| LIST_S dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg REGISTER LIST_E
| LIST_S dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg REGISTER LIST_E
| LIST_S dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg REGISTER LIST_E
| LIST_S dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg REGISTER LIST_E
| LIST_S dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg REGISTER LIST_E
| LIST_S dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg REGISTER LIST_E
| LIST_S dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg REGISTER LIST_E
| LIST_S dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg REGISTER LIST_E
| LIST_S dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg dest_reg REGISTER LIST_E

load_single:
  C_LDR dest_reg s_reg_and_off { printf("Nice.\n"); }
| C_LDR dest_reg s_reg COMMA OFFSET { printf("Nice.\n"); }
;

load_multiple:
  C_LDM {printf("LDM\n");}
;

store_single:
  C_STR dest_reg s_reg_and_off UPDATE { printf("STR!\n"); }
| C_STR dest_reg s_reg_and_off { printf("STR\n"); }
;

store_multiple:
  C_STM M_DB REGISTER UPDATE COMMA reg_list { printf("STM!\n"); }
;

%%
int main(int argc, char **argv) {
  if (argc > 1) {
    if (!(yyin = fopen(argv[1], "r"))) {
      perror(argv[1]);
      return(1);
    }
  }

  printf("Here the parsing happens.\n");
  yyparse();

  return 0;
}

int yyerror(char *s) {
  fprintf(stderr, "error: %s\n", s);
  return 0;
}
