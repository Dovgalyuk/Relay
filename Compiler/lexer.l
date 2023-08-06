%{
#include "parser.h"
#include "tree.h"
%}

/* Scanner for the input file parser */

/* Defines */

DIGIT    [0-9]
BIN      [0-1]
OCT      [0-7]
HEX      [0-9a-f]
VAR      [a-z]

%%

{DIGIT}+ { yylval = new Tree(atoi(yytext)); return T_INT; }
"0x"{HEX}+ { yylval = new Tree((int)strtoul(yytext, NULL, 0)); return T_INT; }
"0b"{BIN}+ return T_INT; // TODO
{VAR}    { yylval = new Tree(yytext[0]); return T_VAR; }
"@"[a-z0-9]+  { yylval = new Tree(strdup(yytext + 1)); return T_FUNC; }

"="      return T_ASSIGN;
"+="     return T_PLUS_ASSIGN;
"-="     return T_MINUS_ASSIGN;

"+"      return T_PLUS;
"-"      return T_MINUS;
"&"      return T_AND;
"|"      return T_OR;
"^"      return T_XOR;

"~"      return T_NEG;
"<<"     return T_SHL;
">>"     return T_SHR;

"C"      return T_C;
"S"      return T_S;
"Z"      return T_Z;

"NC"     return T_NC;
"NS"     return T_NS;
"NZ"     return T_NZ;

"=="     return T_EQUAL;
"!="     return T_NEQUAL;
"<"      return T_LESS;
">"      return T_GREATER;

"!"      return T_NOT;
"&&"     return T_LOG_AND;
"||"     return T_LOG_OR;

IF       return T_IF;
ELSE     return T_ELSE;
WHILE    return T_WHILE;
DO       return T_DO;
ARRAY    return T_ARRAY;
RETURN   return T_RETURN;
OUT      return T_OUT;
IN       return T_IN;
HALT     return T_HALT;

"("      return T_L_PARENT;
")"      return T_R_PARENT;
"["      return T_L_SQUARE;
"]"      return T_R_SQUARE;
"{"      return T_L_CURLY;
"}"      return T_R_CURLY;

","      return T_COMMA;
";"      return T_SEMICOLON;

[ \t\n]+           /* eat up whitespace */
"//"[^\n]*"\n"     /* eat up one-line comments */

.        { printf("Unrecognized character: %s\n", yytext); }

%%

/* C code */

int yywrap()
{
    return 1;
}
