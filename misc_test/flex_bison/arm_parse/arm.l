/* read the startup script */
%{
#include "arm.tab.h"
%}


%%
[ ]             ;
[\n]            { return EOL; }
0x[0-9a-f]+     { yylval.sval = strdup(yytext); return ADDRESS; }
LDR             { return LOAD; }
LDM             { return LOAD_M; }
I               { return INCREMENT; }
D               { return DECREMENT; }
B               { return MOD_B; }
r[0-9][0-5]*    { yylval.sval = strdup(yytext); return REGISTER; }
"{"             { return LIST_S; }
"}"             { return LIST_E; }
"["             { return SQUARE_S; }
"]"             { return SQUARE_E; }
","             { return COMMA; }
.          { printf("Mystery character %c\n", yytext[0]); }
%%
