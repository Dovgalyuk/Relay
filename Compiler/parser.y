/* Grammar file for the compiler */

%{
#include <stdio.h>
#include "lexer.h" /* where yylex is declared */
void yyerror(const char *s);
%}

/* Types and tokens */

%%

/* Rules */

program: /* empty */
;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "%s\n", s);
}
