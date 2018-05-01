/* read the startup script */
%{
#include "arm_openocd.tab.h"
%}


%%
[ ]              ;
[\t]             ;
[\r]             ;
[\n]             { return EOL; }
0x[0-9a-f]{2}    { strcpy(yylval.sval, yytext); return OPCODE; }
0x[0-9a-f]{6}    { strcpy(yylval.sval, yytext); return S_ADDRESS; }
0x[0-9a-f]{8}    { strcpy(yylval.sval, yytext); return ADDRESS; }

ASR              { return C_ASR; }
LSL              { return C_LSL; }
LSR              { return C_LSR; }
ROR              { return C_ROR; }
RRX              { return C_RRX; }

[ \t]PLDW        { return DONTCARE; }
[ \t]PLD         { return C_PLD; }
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
[ \t]CLZ[A-Z]*   { return DONTCARE; }
[ \t]CMP[A-Z]*   { return DONTCARE; }
[ \t]EOR[A-Z]*   { return DONTCARE; }
[ \t]MOV[A-Z]*   { return DONTCARE; }
[ \t]MVN[A-Z]*   { return DONTCARE; }
[ \t]MUL[A-Z]*   { return DONTCARE; }
[ \t]MLA[A-Z]*   { return DONTCARE; }
[ \t][SU]MULL[A-Z]*   { return DONTCARE; }
[ \t][SU]MLAL[A-Z]*   { return DONTCARE; }
[ \t]ORN[A-Z]*   { return DONTCARE; }
[ \t]ORR[A-Z]*   { return DONTCARE; }
[ \t]RFE[A-Z]*   { return DONTCARE; }
[ \t]RSB[A-Z]*   { return DONTCARE; }
[ \t]RSC[A-Z]*   { return DONTCARE; }
[ \t]SBC[A-Z]*   { return DONTCARE; }
[ \t]SUB[A-Z]*   { return DONTCARE; }
[ \t]TEQ[A-Z]*   { return DONTCARE; }
[ \t]TST[A-Z]*   { return DONTCARE; }

[ \t]SXT[A-Z]*   { return DONTCARE; }
[ \t]UXT[A-Z]*   { return DONTCARE; }

[ \t]CDP[A-Z]*   { return DONTCARE; }
[ \t]MRRC[A-Z]*   { return DONTCARE; }
[ \t]MCRR[A-Z]*   { return DONTCARE; }
[ \t]MRC[A-Z]*   { return DONTCARE; }
[ \t]MCR[A-Z]*   { return DONTCARE; }

[ \t]LDC[A-Z]*   { return DONTCARE; }
[ \t]STC[A-Z]*   { return DONTCARE; }

[ \t]CBNZ[A-Z]*  { return DONTCARE; }
[ \t]CBZ[A-Z]*   { return DONTCARE; }
[ \t]BL[A-Z]*    { return DONTCARE; }
[ \t]BX[A-Z]*    { return DONTCARE; }
[ \t]TBH[A-Z]*   { return DONTCARE; }
[ \t]TBB[A-Z]*   { return DONTCARE; }
[ \t]B[A-Z]*     { return DONTCARE; }

[ \t]SVC         { return DONTCARE; }

[ \t]NOP         { return NOP; }

"UNDEFINED INSTRUCTION" { return UND; }
"UNDEFINED" { return UND; }

[A-Z]+           { return UNKNOWN; }

[-]?r[0-9][0-5]* { strcpy(yylval.sval, yytext); return REGISTER; }
[-]?c[0-9][0-5]* { strcpy(yylval.sval, yytext); return REG_C; }
[-]?p[0-9][0-5]* { strcpy(yylval.sval, yytext); return REG_P; }
#[-]?0x[0-9a-f]* { strcpy(yylval.sval, yytext); return OFFSET; }
[-]?[0-9]+       { strcpy(yylval.sval, yytext); return IMMEDIATE; }
#[-]?[0-9]+      { strcpy(yylval.sval, yytext); return FOOBAR; }
"{"              { return LIST_S; }
"}"              { return LIST_E; }
"["              { return SQUARE_S; }
"]"              { return SQUARE_E; }
","              { return COMMA; }
"!"              { return UPDATE; }
.          { printf("Mystery character %c\n", yytext[0]); }
%%
