/* read the startup script */
%{
#include "arm.tab.h"
%}


%%
[ ]              ;
[\t]             ;
[\r]             ;
[\n]             { return EOL; }
0x[0-9a-f]+      { yylval.sval = strdup(yytext); return ADDRESS; }

ASR              { return C_ASR; }
LSL              { return C_LSL; }
LSR              { return C_LSR; }
ROR              { return C_ROR; }
RRX              { return C_RRX; }

[ \t]LDR[A-Z]*              { return C_LDR; }
[ \t]STR[A-Z]*              { return C_STR; }

LDM[A-Z]*              { return C_LDM; }
POP              { return C_POP; }
PUSH             { return C_PUSH; }
STM[A-Z]*              { return C_STM; }

[A-Z]+           { return DONTCARE; }

"UNDEFINED INSTRUCTION" { return UND; }

r[0-9][0-5]*     { yylval.sval = strdup(yytext); return REGISTER; }
#[-]?0x[0-9a-f]* { yylval.sval = strdup(yytext); return OFFSET; }
"{"              { return LIST_S; }
"}"              { return LIST_E; }
"["              { return SQUARE_S; }
"]"              { return SQUARE_E; }
","              { return COMMA; }
"!"              { return UPDATE; }
.          { printf("Mystery character %c\n", yytext[0]); }
%%
