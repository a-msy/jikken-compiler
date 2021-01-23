%{
 int num_lines = 0, num_chars = 0;
 int char_count = 0, num_count = 0;
%}
%%
\n ++num_lines; ++num_chars;
[a-zA-Z] ++char_count; ++num_chars;
[0-9] ++num_count; ++num_chars;
. ++num_chars;
%%
int main (void) {
 yylex();
 printf("# of lines = %d, # of chars = %d\n", num_lines,num_chars);
 printf("# of charasters = %d, # of numbers = %d\n", char_count,num_count);
 return 0;
} 