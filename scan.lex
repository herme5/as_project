%{
#include "machine.h"
#include "expr.h"
#include "parser.tab.h"
%}
%option noyywrap
%%

let {return LET;}
if {return IF;}
then {return THEN;}
else {return ELSE;}

[[:digit:]] {yylval.num = atoi(yytext); return NB;}
[[:alpha:]] {yylval.id= strdup(yytext); return ID;}

[[:space:]] {;}

"+"  {return PLUS;}
"-"  {return MINUS;}
"*"  {return MULT;}
"/"  {return DIV;}

"<"  {return LE;}
">"  {return GE;}
"<=" {return LEQ;}
">=" {return GEQ;}
"==" {return EQ;}

"&&" {return AND;}
"||" {return OR;}
"!"  {return NOT;}

[=()] {return yytext[0];}
; {return FIN_EXPR;}

%%
