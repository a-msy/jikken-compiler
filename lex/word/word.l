%{
 int num_lines = 0, num_chars = 0, wordCount = 0;
%}
%%
[^ \t\n]+ wordCount++; num_chars += yyleng;
\n ++num_lines; ++num_chars;
. ++num_chars;
%%
int main(void) {
 yylex();
 printf("# of lines = %d, # of chars = %d, # of words = %d\n",num_lines, num_chars, wordCount);
 return 0;
} 