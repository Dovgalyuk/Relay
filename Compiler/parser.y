%code requires
{
#include "node.h"
#define YYSTYPE Node *
}

%{
#include <stdio.h>
#include "lexer.h" /* where yylex is declared */
void yyerror(const char *s);
class Node;
extern Node *root;
%}

/* Types and tokens */
%token T_INT
%token T_VAR
%token T_FUNC
%token T_ASSIGN
%token T_PLUS T_MINUS T_AND T_OR T_XOR
%token T_NEG T_SHL T_SHR
%token T_C T_NC T_Z T_NZ T_S T_NS
%token T_EQUAL T_NEQUAL T_LESS T_GREATER
%token T_NOT T_LOG_AND T_LOG_OR
%token T_IF T_WHILE T_DO T_ARRAY T_RETURN
%token T_L_PARENT T_R_PARENT
%token T_L_SQUARE T_R_SQUARE
%token T_L_CURLY T_R_CURLY
%token T_COMMA T_SEMICOLON

%left T_PLUS T_MINUS T_AND T_OR T_XOR
%left T_LOG_AND T_LOG_OR

%%

/* Rules */

program:
    functions { root = $1; }
;

functions:
    /* empty */ { $$ = new Node(NodeType::LIST); }
    | function functions { $$ = $2; $$->addFirstChild($1); }
;

function:
    T_FUNC T_L_PARENT function_args T_R_PARENT
    code_block
    { $$ = $1; $$->addChild($5); }
;

function_args:
;

code_block:
    T_L_CURLY statements T_R_CURLY { $$ = new Node(NodeType::SCOPE); $$->merge($2); }
;

statements:
    /* empty */ { $$ = new Node(NodeType::LIST); }
    | statement statements { $$ = $2; $$->addFirstChild($1); }
;

statement:
    T_SEMICOLON
    | stat_assign
    | code_block
;

stat_assign:
    vars T_ASSIGN rvalue T_SEMICOLON
    {
        $$ = new Node(NodeType::ASSIGN);
        $$->addChild($1);
        $$->addChild($3);
    }
;

vars:
    T_VAR
    {
        $$ = new Node(NodeType::LIST);
        $$->addChild($1);
    }
    | T_VAR vars { $$ = $2; $$->addChild($1); }
;

rvalue:
    vars_int
;

vars_int:
    var_int vars_int_r
    {
        $$ = $2;
        $$->addChild($1);
    }
;

vars_int_r:
    /* empty */ { $$ = new Node(NodeType::LIST); }
    | var_int vars_int_r
    {
        $$ = $2;
        $$->addChild($1);
    }
;

var_int:
    T_VAR
    | T_INT
;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "%s\n", s);
}
