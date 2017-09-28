/* Startup Script */
/* Copied from old plumber work. */
/* Test for if it would be possible to transition drseus to using flex/bison */

%{
#include <stdio.h>

extern FILE *yyin;

int yylex();
int yyerror(char *s);

%}

%union {
  int ival;
  float fval;
  char *sval;
}

%token <sval> ADDRESS
%token EOL
%token LOAD
%token LOAD_M
%token INCREMENT
%token DECREMENT
%token MOD_B
%token <sval> REGISTER
%token LIST_S
%token LIST_E
%token SQUARE_S
%token SQUARE_E
%token COMMA

%%
line:
| line ADDRESS ADDRESS command EOL { printf("Is that a line?\n"); }
| line EOL
;

command:
  load REGISTER COMMA SQUARE_S REGISTER SQUARE_E { printf("Nice.\n"); }
| load REGISTER COMMA LIST_S REGISTER COMMA REGISTER COMMA REGISTER COMMA REGISTER LIST_E { printf("Should make a register list a separate token\n"); }
;

load:
LOAD {printf("LOAD\n");}
| LOAD MOD_B {printf("LOAD MOD_B\n");}
| LOAD_M {printf("LOAD_M\n");}
| LOAD_M in_dec MOD_B {printf("LOAD_M in_dec MOD_B\n");}
;

in_dec:
  INCREMENT
| DECREMENT
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
