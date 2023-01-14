%code requires
{
#include "tree.h"
#define YYSTYPE Tree *
}

%{
#include <stdio.h>
#include "lexer.h" /* where yylex is declared */
void yyerror(const char *s);
class Tree;
extern Tree *root;
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
%token T_IF T_ELSE T_WHILE T_DO T_ARRAY T_RETURN
%token T_L_PARENT T_R_PARENT
%token T_L_SQUARE T_R_SQUARE
%token T_L_CURLY T_R_CURLY
%token T_COMMA T_SEMICOLON

%left T_PLUS T_MINUS T_AND T_OR T_XOR
%left T_LOG_AND T_LOG_OR
%left T_ELSE T_IF

%%

/* Rules */

program:
    functions { root = $1; }
;

functions:
    /* empty */ { $$ = new Tree(TreeType::LIST); }
    | function functions { $$ = $2; $$->addFirstChild($1); }
;

function:
    T_FUNC T_L_PARENT function_args T_R_PARENT
    code_block
    { $$ = $1; $$->addChild($5); }
;

function_args:
    /* empty */
    | T_VAR function_args
;

code_block:
    T_L_CURLY statements T_R_CURLY { $$ = new Tree(TreeType::SCOPE); $$->merge($2); }
;

statements:
    /* empty */ { $$ = new Tree(TreeType::LIST); }
    | statement statements { $$ = $2; $$->addFirstChild($1); }
;

statement:
    open_statement
    | closed_statement
;

open_statement:
    if_header statement { $$->addChild($2); }
    | if_header closed_statement T_ELSE open_statement
      { $$->addChild($2); $$->addChild($4); }
    | while_header open_statement
      { $$->addChild($2); }
;

closed_statement:
    non_if_statement
    | if_header closed_statement T_ELSE closed_statement
      { $$->addChild($2); $$->addChild($4); }
    | while_header closed_statement
      { $$->addChild($2); }
;

if_header:
    T_IF T_L_PARENT cond T_R_PARENT
    { $$ = new Tree(TreeType::IF, $3); }
;

while_header:
    T_WHILE T_L_PARENT cond T_R_PARENT
    { $$ = new Tree(TreeType::WHILE, $3); }
;

non_if_statement:
    T_SEMICOLON
    | stat_assign
    | stat_return
    | stat_prefix
    | code_block
;

stat_assign:
    vars T_ASSIGN rvalue T_SEMICOLON
    {
        $$ = new Tree(TreeType::ASSIGN);
        $$->addChild($1);
        $$->addChild($3);
    }
;

stat_return:
    T_RETURN rvalue T_SEMICOLON
    { $$ = new Tree(TreeType::RETURN); $$->addChild($2); }
;

stat_prefix:
    prefix_expr T_SEMICOLON
;

cond:
    cond_and
    | cond_and T_LOG_OR cond
    { $$ = new Tree(TreeType::OR, $1, $3); }
;

cond_and:
    cond_simple
    | cond_simple T_LOG_AND cond_and
    { $$ = new Tree(TreeType::AND, $1, $3); }
;

cond_simple:
    flag_check T_L_PARENT expr T_R_PARENT
      { $$ = $1; $$->addChild($3); }
    | vars T_NEQUAL vars_int
      { $$ = new Tree(TreeType::NEQUAL, $1, $3); }
;

flag_check:
    T_C { $$ = new Tree(TreeType::CARRY); }
    | T_NC { $$ = new Tree(TreeType::NCARRY); }
    | T_Z { $$ = new Tree(TreeType::ZERO); }
    | T_NZ { $$ = new Tree(TreeType::NZERO); }
    | T_S { $$ = new Tree(TreeType::SIGN); }
    | T_NS { $$ = new Tree(TreeType::NSIGN); }
;

vars:
    T_VAR
    {
        $$ = new Tree(TreeType::LIST);
        $$->addChild($1);
    }
    | T_VAR vars { $$ = $2; $$->addChild($1); }
;

rvalue:
    expr
;

expr:
    vars_int
    | T_L_PARENT expr T_R_PARENT { $$ = $2; }
    | prefix_expr
    | vars_int T_PLUS expr
      {
          $$ = new Tree(TreeType::ADD);
          $$->addChild($1);
          $$->addChild($3);
      }
;

prefix_expr:
    shift_op vars_int { $$->addChild($2); }
;

shift_op:
    T_SHL { $$ = new Tree(TreeType::LSHIFT); }
    | T_SHR { $$ = new Tree(TreeType::RSHIFT); }
;

vars_int:
    var_int vars_int_r
    {
        $$ = $2;
        $$->addChild($1);
    }
;

vars_int_r:
    /* empty */ { $$ = new Tree(TreeType::LIST); }
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
