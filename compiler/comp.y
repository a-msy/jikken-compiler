%{
#include <stdio.h>
#include "ast.h"
#include "comp.tab.h"

extern Node* top;
extern int yylex();
extern int yyerror();
%}
%union{
    struct node* np;
    char* sp;
    int ival;
}
%token DEFINE ARRAY WHILE IF ELSE SEMIC FOR
%token L_BRACKET R_BRACKET L_PARAN R_PARAN L_BRACE R_BRACE
%token ASSIGN 
%token <sp>IDENT
%token BREAK ADD SUB MUL DIV EQ LT GT LTE GLT INCRE DECRE SUR
%token <ival>NUMBER
%token FUNC COMMA FUNCCALL
%type <np> program declarations idents argument_calllist argument_dcllist function_call function_dcl argument_dcl
%type <np> array_index decl_statement statement statements assignment_stmt
%type <np> expression term factor var while_loop
%type <np> for_loop loop_stmt if_stmt cond_stmt condition break_stmt else_stmt elif_stmt
%type <ival> add_op mul_op crement cond_op
%%
program : declarations statements { top = build_node2(PROGRAM_AST, $1, $2); }
|   declarations { top = build_node1(PROGRAM_AST, $1); }
;
declarations : decl_statement declarations { $$ = build_node2(DECLARATIONS_AST, $1, $2); }
| decl_statement { $$ = build_node1(DECLARATIONS_AST, $1);}
;
decl_statement : DEFINE idents SEMIC { $$ = build_node1(DEFINE_AST, $2); }
| ARRAY IDENT array_index SEMIC { $$ = build_node1(DEFINE_ARRAY_AST, build_array_node(ARRAY_AST, $2, $3)); }
| function_dcl { $$ = build_node1(FUNCTION_DCL_AST, $1); }
;
idents : IDENT COMMA idents { $$ = build_node2(IDENTS_AST, build_ident_node(IDENT_AST,$1), $3); }
| IDENT { $$ = build_node1(IDENTS_AST, build_ident_node(IDENT_AST, $1)); }
;
statements : statement statements { $$ = build_node2(STATEMENTS_AST, $1, $2); } 
| statement { $$ = build_node1(STATEMENTS_AST, $1); }
;
statement : assignment_stmt { $$ = build_node1(STATEMENT_AST, $1); }
| loop_stmt { $$ = build_node1(STATEMENT_AST, $1); }
| cond_stmt { $$ = build_node1(STATEMENT_AST, $1); }
| function_call { $$ = build_node1(STATEMENT_AST, $1); }
| break_stmt { $$ = build_node1(STATEMENT_AST, $1); };
;
assignment_stmt : IDENT ASSIGN expression SEMIC { $$ = build_node2(ASSIGNMENT_STMT_AST, build_ident_node(IDENT_AST, $1), $3); }
| IDENT array_index ASSIGN expression SEMIC { $$ = build_node2(ASSIGNMENT_STMT_AST, build_array_node(IDENT_AST, $1, $2), $4); }
;
array_index : array_index L_BRACKET expression R_BRACKET { $$ = build_node2(ARRAY_INDEX_AST, $1, $3); }
| L_BRACKET expression R_BRACKET { $$ = build_node1(ARRAY_INDEX_AST, $2); }
| L_BRACKET R_BRACKET{ $$ = build_node1(ARRAY_INDEX_AST, 0); }
;
expression : expression add_op term
    { 
        if($2 == OP_ADD){
            $$ = build_node2(ADD_AST, $1, $3); 
        }else{
            $$ = build_node2(SUB_AST, $1, $3); 
        }
    }
| expression crement
    { 
        if($2 == OP_INCRE){
            $$ = build_node1(INCRE_AST, $1); 
        }else{
            $$ = build_node1(DECRE_AST, $1);
        }
    }
| crement expression
    { 
        if($1 == OP_INCRE){
            $$ = build_node1(INCRE_AST, $2); 
        }else{
            $$ = build_node1(DECRE_AST, $2);
        }
    }
| term
;
term : term mul_op factor 
    { 
        if($2 == OP_MUL)
        {
            $$ = build_node2(MUL_AST, $1, $3); 
        }
        else if($2 == OP_DIV)
        {
            $$ = build_node2(DIV_AST, $1, $3); 
        }
        else
        {
            $$ = build_node2(SUR_AST, $1, $3);
        }
    }
| factor
;
factor : var
| L_PARAN expression R_PARAN { $$ = build_node1(FACTOR_AST, $2); }
;
add_op : ADD { $$ = OP_ADD; }
| SUB { $$ = OP_SUB; }
;
mul_op : MUL { $$ = OP_MUL; }
| DIV { $$ = OP_DIV; }
| SUR { $$ = OP_SUR; }
;
crement : INCRE { $$ = OP_INCRE; }
| DECRE { $$ = OP_DECRE; }
;
var : IDENT { $$ = build_ident_node(IDENT_AST, $1); }
| NUMBER { $$ = build_num_node(NUMBER_AST, $1); }
| IDENT array_index { $$ = build_array_node(ARRAY_AST, $1, $2); }
;
loop_stmt : while_loop
| for_loop
;
while_loop : WHILE L_PARAN condition R_PARAN L_BRACE statements R_BRACE { $$ = build_node2(WHILE_LOOP_AST, $3, $6); }
| WHILE L_PARAN condition R_PARAN SEMIC { $$ = build_node1(WHILE_LOOP_AST, $3); }
;
for_loop : FOR L_PARAN assignment_stmt condition SEMIC expression R_PARAN L_BRACE statements R_BRACE { $$ = build_node4(FOR_LOOP_AST, $3, $4, $6, $9); }
| FOR L_PARAN SEMIC SEMIC R_PARAN L_BRACE statements R_BRACE { $$ = build_node1(FOR_LOOP_AST, $7); }
;
cond_stmt : if_stmt { $$ = build_node1(IF_AST, $1); }
| if_stmt else_stmt { $$ = build_node2(IF_AST, $1, $2); }
| IF L_PARAN condition R_PARAN statement { $$ = build_node2(IF_AST, $3, $5); }
;
if_stmt : IF L_PARAN condition R_PARAN L_BRACE statements R_BRACE { $$ = build_node2(IF_STMT_AST, $3, $6); }
;
else_stmt : elif_stmt else_stmt { $$ = build_node2(ELSE_STMT_AST, $1, $2); };
| ELSE L_BRACE statements R_BRACE { $$ = build_node1(ELSE_STMT_AST, $3); }
;
elif_stmt : ELSE if_stmt { $$ = build_node1(ELIF_STMT_AST, $2); };
;
condition : expression cond_op expression {
    if($2 == OP_EQ){
        $$ = build_node2(EQ_AST, $1, $3);
    }else if($2 == OP_LT){
        $$ = build_node2(LT_AST, $1, $3);
    }else if($2 == OP_GT){
        $$ = build_node2(GT_AST, $1, $3);
    }else if($2 == OP_LTE){
        $$ = build_node2(LTE_AST, $1, $3);
    }else if($2 == OP_GLT){
        $$ = build_node2(GLT_AST, $1, $3);
    }
}
;
cond_op : EQ { $$ = OP_EQ; }
| LT { $$ = OP_LT; }
| GT { $$ = OP_GT; }
| LTE { $$ = OP_LTE; }
| GLT { $$ = OP_GLT; }
;
function_dcl : FUNC IDENT L_PARAN argument_dcllist R_PARAN L_BRACE declarations statements R_BRACE { $$ = build_node4(FUNCTION_DCL_AST, build_ident_node(IDENT_AST, $2), $4, $7, $8); }
;
argument_dcllist : argument_dcl COMMA argument_dcllist { $$ = build_node2(ARGUMENT_DCLLIST_AST, $1, $3); }
| argument_dcl { $$ = build_node1(ARGUMENT_DCLLIST_AST, $1); }
| { $$ = build_node1(ARGUMENT_DCLLIST_AST, 0); }
;
argument_dcl : DEFINE IDENT { $$ = build_node1(ARGUMENT_DCL_AST, build_ident_node(IDENT_AST, $2)); }
| ARRAY IDENT array_index { $$ = build_node1(ARGUMENT_DCL_AST, build_array_node(ARRAY_AST, $2, 0)); }
;
function_call : FUNCCALL IDENT L_PARAN argument_calllist R_PARAN SEMIC { $$ = build_node2(FUNCTION_CALL_AST, build_ident_node(IDENT_AST, $2), $4); }
;
argument_calllist : expression COMMA argument_calllist { $$ = build_node2(ARGUMENT_CALLLIST_AST, $1, $3); }
| expression { $$ = build_node1(ARGUMENT_CALLLIST_AST, $1); }
| { $$ = build_node1(ARGUMENT_CALLLIST_AST, 0); }
;
break_stmt : BREAK SEMIC  {$$ = build_node1(BREAK_AST, 0);}
;
%%