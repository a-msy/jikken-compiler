%{
#include <stdio.h>
#include "arithrv.tab.h"
 extern int yylex();
 extern int yyerror();
 double vbltable[26];
 %}
%union{
 double dval;
 int vblno;
 }
%token <vblno> NAME
%token <dval> NUMBER
%type <dval> expression mulexp primary
%%
statement_list : statement '\n'
| statement_list statement '\n'
;
statement : NAME '=' expression {vbltable[$1] = $3; printf("%c = %g\n> ", $1 + $3); }
| expression {printf("= %g\n> ", $1);}
;
expression : expression '+' mulexp {$$ = $1 + $3; }
| expression '-' mulexp {$$ = $1 - $3; }
| mulexp {$$ = $1;}
;
mulexp : mulexp '*' primary {$$ = $1 * $3; }
| mulexp '/' primary
{if($3 == 0) {/* printf("divide by zero\n"); or */ yyerror("divide by zero"); return 0;}
 else $$ = $1 / $3; }
| primary {$$ = $1;}
;
primary : '(' expression ')' {$$ = $2; }
| NUMBER {$$ = $1;}
| NAME {$$ = vbltable[$1];}
;
%%
int main(void)
{
 printf("> ");
 if (yyparse()) {
 fprintf(stderr, "Error\n");
 return 1;
 }
} 