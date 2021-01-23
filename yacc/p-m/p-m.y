%{
    #include <stdio.h>
    #include "p-m.tab.h"
    extern int yylex();
    extern int yyerror();
%}
    %token NUMBER
%%
    statement : expression {  printf("= %d\n", $1);  };
    expression : expression '+' NUMBER {$$ = $1 + $3; } | expression '-' NUMBER {$$ = $1 - $3; } | NUMBER { $$ = $1; };
%%
int main(void)
{
    if (yyparse()) {
        fprintf(stderr, "Error\n");
        return 1;
    }
    return 0;
}