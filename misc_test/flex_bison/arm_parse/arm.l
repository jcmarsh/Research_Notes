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

[ \t]ADC              { return C_ADC; }
[ \t]ADD              { return C_ADD; }
[ \t]ADR              { return C_ADR; }
[ \t]AND              { return C_AND; }
[ \t]BIC              { return C_BIC; }
[ \t]CMN              { return C_CMN; }
[ \t]CMP              { return C_CMP; }
[ \t]EOR              { return C_EOR; }
[ \t]MOV              { return C_MOV; }
[ \t]MVN              { return C_MVN; }
[ \t]ORN              { return C_ORN; }
[ \t]ORR              { return C_ORR; }
[ \t]RSB              { return C_RSB; }
[ \t]RSC              { return C_RSC; }
[ \t]SBC              { return C_SBC; }
[ \t]SUB              { return C_SUB; }
[ \t]TEQ              { return C_TEQ; }
[ \t]TST              { return C_TST; }

ASR              { return C_ASR; }
LSL              { return C_LSL; }
LSR              { return C_LSR; }
ROR              { return C_ROR; }
RRX              { return C_RRX; }

[ \t]CBNZ             { return C_CBNZ; }
[ \t]CBZ              { return C_CBZ; }
[ \t]BL               { return C_BL; }
[ \t]BLX              { return C_BLX; }
[ \t]BX               { return C_BX; }
[ \t]BXJ              { return C_BXJ; }
[ \t]TBH              { return C_BXJ; }
[ \t]TBB              { return C_BXJ; }
[ \t]B                { return C_B; }

EQ                    { return COND_EQ; }
NE                    { return COND_NE; }
CS                    { return COND_CS; }
CC                    { return COND_CC; }
MI                    { return COND_MI; }
PL                    { return COND_PL; }
VS                    { return COND_VS; }
VC                    { return COND_VC; }
HI                    { return COND_HI; }
LS                    { return COND_LS; }
GE                    { return COND_GE; }
LT                    { return COND_LT; }
GT                    { return COND_GT; }
LE                    { return COND_LE; }

[ \t]LDR              { return C_LDR; }
[ \t]STR              { return C_STR; }

H                { return M_HALF; }
S                { return M_SIGNED; }
B                { return M_BYTE; }
D                { return M_DOUBLE; }
T                { return M_UNPRIV; }
X                { return M_EXCLU; }

LDM              { return C_LDM; }
POP              { return C_POP; }
PUSH             { return C_PUSH; }
STM              { return C_STM; }

IA               { return M_IA; }
DA               { return M_DA; }
FD               { return M_FD; }
FA               { return M_FA; }
DB               { return M_DB; }
IB               { return M_IB; }
EA               { return M_EA; }
ED               { return M_ED; }

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
