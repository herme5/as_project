%{
#include "parser.tab.h"
%}

%option noyywrap
%%

let   {return LET;}
if    {return IF;}
then  {return THEN;}
else  {return ELSE;}
fun   {return TFUN;}
in    {return IN;}
where {return WHERE;}

[[:digit:]]+ {yylval.num = atoi(yytext); return TNUM;}
[[:alpha:]]+ {yylval.id= strdup(yytext); return TID;}

[[:space:]] {;}
"->" {return ARROW;}


"<"  {return TLE;}
">"  {return TGE;}
"<=" {return TLEQ;}
">=" {return TGEQ;}
"==" {return TEQ;}

"&&" {return TAND;}
"||" {return TOR;}
"!"  {return TNOT;}

[+\-*/=()] {return yytext[0];}
; {return FIN_EXPR;}
