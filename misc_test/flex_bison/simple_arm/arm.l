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

[ \t]LDR[A-Z]*   { return C_LDR; }
[ \t]STR[A-Z]*   { return C_STR; }

LDM[A-Z]*        { return C_LDM; }
POP              { return C_POP; }
PUSH             { return C_PUSH; }
STM[A-Z]*        { return C_STM; }

[ \t]ADC[A-Z]*   { return DONTCARE; }
[ \t]ADD[A-Z]*   { return DONTCARE; }
[ \t]ADR[A-Z]*   { return DONTCARE; }
[ \t]AND[A-Z]*   { return DONTCARE; }
[ \t]BIC[A-Z]*   { return DONTCARE; }
[ \t]CMN[A-Z]*   { return DONTCARE; }
[ \t]CMP[A-Z]*   { return DONTCARE; }
[ \t]EOR[A-Z]*   { return DONTCARE; }
[ \t]MOV[A-Z]*   { return DONTCARE; }
[ \t]MVN[A-Z]*   { return DONTCARE; }
[ \t]ORN[A-Z]*   { return DONTCARE; }
[ \t]ORR[A-Z]*   { return DONTCARE; }
[ \t]RSB[A-Z]*   { return DONTCARE; }
[ \t]RSC[A-Z]*   { return DONTCARE; }
[ \t]SBC[A-Z]*   { return DONTCARE; }
[ \t]SUB[A-Z]*   { return DONTCARE; }
[ \t]TEQ[A-Z]*   { return DONTCARE; }
[ \t]TST[A-Z]*   { return DONTCARE; }

[ \t]CBNZ[A-Z]*  { return DONTCARE; }
[ \t]CBZ[A-Z]*   { return DONTCARE; }
[ \t]BL[A-Z]*    { return DONTCARE; }
[ \t]BX[A-Z]*    { return DONTCARE; }
[ \t]TBH[A-Z]*   { return DONTCARE; }
[ \t]TBB[A-Z]*   { return DONTCARE; }
[ \t]B[A-Z]*     { return DONTCARE; }

[A-Z]+           { return UNKNOWN; }

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
