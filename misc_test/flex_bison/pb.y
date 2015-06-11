/* Startup Script */

%{
#include <stdio.h>

extern FILE *yyin;

%}

%token BENCH
%token BI
%token SINGLE
%token COMPONENT
%token ARGS
%token EOL

%%
line:
| line relationship EOL { }
| line invocation EOL { }
;

relationship:
  BENCH BI comp { printf("The benches: Bench launches %s\n", $3); }
| comp BI comp { printf("bi between %s and %s\n", $1, $3); }
| comp SINGLE comp { printf("single between %s and %s\n", $1, $3); }
;

invocation:
  comp ARGS { printf("Invoking something: %s with args %s\n", $1, $2); }
;

comp:
  COMPONENT { $$ = $1; }
;

%%
int main(int argc, char **argv) {
  if (argc > 1) {
    if (!(yyin = fopen(argv[1], "r"))) {
      perror(argv[1]);
      return(1);
    }
  }

  yyparse();

  return 0;
}

yyerror(char *s) {
  fprintf(stderr, "error: %s\n", s);
}
