%{
    #include <stdio.h>
    #define YYSTYPE double
    #include "arithr.tab.h"
    extern int yylex();
    extern int yyerror();
%}
%token NUMBER
%%
    statement : expression {printf("= %g\n", $1);}
    ;
    expression : expression '+' mulexp {$$ = $1 + $3;}
    | expression '-' mulexp {$$ = $1 - $3;}
    | mulexp {$$ = $1;}
    ;
    mulexp : mulexp '*' primary {$$ = $1 * $3;}
    | mulexp '/' primary { 
        if ($3 == 0) {
            yyerror("divide by zero"); 
            return 0;
        }
        else $$ = $1 / $3;
    }
    | primary {$$ = $1;}
    ;
    primary : '(' expression ')' {$$ = $2;}
    | NUMBER {$$ = $1;}
    ;
%%
int main(void)
{
    if (yyparse()) {
        fprintf(stderr, "Error\n");
        return 1;
    }
    return 0;
}