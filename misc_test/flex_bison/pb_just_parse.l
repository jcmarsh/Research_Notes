/* read the startup script */
%{
#include <stdio.h>
%}

%%
[ \t\r]
"Bench"    { printf("BENCH: %s\n", yytext); }
"<->"      { printf("BI: %s\n", yytext); }
"-->"      { printf("SINGLE: %s\n", yytext); }
("::".*)   { printf("ARGS: %s\n", yytext); }
[a-zA-Z_]+ { printf("COMPONENT: %s\n", yytext); }
\n         { printf("EOL\n"); }
.          { printf("Error\n"); }
%%

main(int argc, char **argv) {
  yylex();
}


