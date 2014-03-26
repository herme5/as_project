%{
#include "parser.tab.h"
%}

%option noyywrap
%%

let   {return T_LET;}
if    {return T_IF;}
then  {return T_THEN;}
else  {return T_ELSE;}
fun   {return T_FUN;}
in    {return T_IN;}
where {return T_WHERE;}

[[:digit:]]+ {yylval.num = atoi(yytext); return T_NUM;}
[[:alpha:]]+ {yylval.id= strdup(yytext); return T_ID;}

[[:space:]] {;}
"->" {return T_ARROW;}


"<"  {return T_LE;}
">"  {return T_GE;}
"<=" {return T_LEQ;}
">=" {return T_GEQ;}
"==" {return T_EQ;}

"&&" {return T_AND;}
"||" {return T_OR;}
"!"  {return T_NOT;}

[+\-*/=()] {return yytext[0];}
; {return EOE;}
