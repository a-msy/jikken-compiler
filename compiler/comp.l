%{
#include "comp.tab.h"
#include <string.h>
 int num_lines = 0;
/* lex for comp.y */
%}
%%
"define" return DEFINE;
"array" return ARRAY;
"while" return WHILE;
"for" return FOR;
"if" return IF;
"else" return ELSE;
"func" return FUNC;
"break" return BREAK;
"funccall" return FUNCCALL;
"," return COMMA;
";" return SEMIC;
"[" return L_BRACKET;
"]" return R_BRACKET;
"(" return L_PARAN;
")" return R_PARAN;
"{" return L_BRACE;
"}" return R_BRACE;
"=" return ASSIGN;
"+" return ADD;
"++" return INCRE;
"--" return DECRE;
"-" return SUB;
"*" return MUL;
"/" return DIV;
"%" return SUR;
"==" return EQ;
"<" return LT;
">" return GT;
"<=" return LTE;
">=" return GLT;
[a-zA-Z][a-zA-Z0-9]* {
    yylval.sp = (char*)malloc(sizeof(char) * yyleng);
    strncpy(yylval.sp, yytext, yyleng);
    return IDENT; }
[0-9]+ { yylval.ival = atoi(yytext); return NUMBER; }
[ \t\n];
%%