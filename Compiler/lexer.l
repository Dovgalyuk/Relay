/* Scanner for the input file parser */

/* Defines */

DIGIT    [0-9]
ID       [a-z][a-z0-9]*

%%

{DIGIT}+ { printf("Integer"); }

%%

/* C code */

int yywrap()
{
    return 1;
}
