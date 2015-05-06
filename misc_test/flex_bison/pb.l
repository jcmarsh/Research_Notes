/* read the startup script */
%{
#include "pb.tab.h"
%}

%%
[ \t\r]
"Bench"    { return BENCH; }
"<->"      { return BI; }
"-->"      { return SINGLE; }
("::".*)   { yylval = strdup(yytext); return ARGS; }
[a-zA-Z_]+ { yylval = strdup(yytext); return COMPONENT; }
\n         { return EOL; }
.          { yyerror("Mystery character %c\n", *yytext); }
%%
