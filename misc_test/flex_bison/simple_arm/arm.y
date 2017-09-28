/* Startup Script */
/* Copied from old plumber work. */
/* Test for if it would be possible to transition drseus to using flex/bison */

%{
#include <stdio.h>
#include "arm.h"

extern FILE *yyin;
typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

int yylex();
int yyerror(char *s);

int l_count = 0;

char registers[200] = {0};
 %}

%union {
  int ival;
  float fval;
  char *sval;
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

 // Values
%token <sval> ADDRESS
 //%token <sval> REGISTER
%token <sval> REGISTER
%token <sval> OFFSET
%type<sval> dest_reg
%type<sval> s_reg
%type<sval> s_reg_and_off
%type<sval> reg_and_off
%type<sval> rec_reg
%type<sval> store_single

%token UND
%token DONTCARE

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
;

dont_care:
DONTCARE ADDRESS
| DONTCARE REGISTER
| DONTCARE REGISTER COMMA REGISTER
| DONTCARE REGISTER COMMA OFFSET
| DONTCARE REGISTER COMMA REGISTER COMMA REGISTER
| DONTCARE REGISTER COMMA REGISTER COMMA OFFSET
;

load_store:
load_single
| load_multiple
| store_single
| store_multiple
;

dest_reg:
REGISTER COMMA { $$ = $1; }
;

//reg_and_reg:
//  REGISTER COMMA REGISTER
//;

reg_and_off:
REGISTER COMMA OFFSET { sprintf($$, "%s, %s", $1, $3); }
;

s_reg_and_off:
SQUARE_S reg_and_off SQUARE_E { $$ = $2; }
;

s_reg:
SQUARE_S REGISTER SQUARE_E { $$ = $2; }
;

rec_reg:
REGISTER { $$ = $1; }
| rec_reg COMMA REGISTER { sprintf($$, "%s, %s", $1, $3); }
;

load_single:
C_LDR dest_reg s_reg_and_off { printf("LDR. dest: %s args: %s\n", $2, $3); }
| C_LDR dest_reg s_reg COMMA OFFSET { printf("LDR. dest: %s args: %s %s\n", $2, $3, $5); }
;

load_multiple:
  C_LDM {printf("LDM\n");}
;

store_single:
C_STR dest_reg s_reg_and_off UPDATE { printf("STR! dest_reg: %s args %s\n", $2, $3); sprintf(registers, "%s", $3); }
| C_STR dest_reg s_reg_and_off { printf("STR dest_reg: %s args %s\n", $2, $3); sprintf(registers, "%s",  $3);}
;

store_multiple:
C_STM REGISTER UPDATE COMMA LIST_S rec_reg LIST_E { printf("STM! dest: %s list: %s\n", $2, $6); }
| C_STM dest_reg LIST_S rec_reg LIST_E { printf("STM dest: %s list: %s\n", $2, $4); }
;

%%
int parse_line(char *result, char *parse) {
  YY_BUFFER_STATE state;
  state = yy_scan_string(parse);
  yyparse();
  yy_delete_buffer(state);

  sprintf(result, "%s", registers);
  return 0;
}

/*
int main(int argc, char **argv) {  
  if (argc > 1) {
    if (!(yyin = fopen(argv[1], "r"))) {
      perror(argv[1]);
      return(1);
    }

    yyparse();
  } else {
    char result[200] = {0};
    parse_line(result, "0x00100608	0xe50b3014	STR r3, [r11, #-0x14]\n");
    printf("Result: %s\n", result);
  }

  return 0;
}
*/

int yyerror(char *s) {
  fprintf(stderr, "error: %s\n", s);
  return 0;
}
