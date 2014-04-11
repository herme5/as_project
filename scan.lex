%{
#include "parser.tab.h"

  char *c;

%}

%option noyywrap

%x PRINT
%x COM
%%

let   {return T_LET;}
if    {return T_IF;}
then  {return T_THEN;}
else  {return T_ELSE;}
fun   {return T_FUN;}
in    {return T_IN;}
where {return T_WHERE;}

rec {return T_REC;}

cons  {return T_CONS;}
head  {return T_HEAD;}
tail  {return T_TAIL;}
append {return T_APPEND;}

<INITIAL>print\" {c=malloc(10000*sizeof(char)); BEGIN PRINT;}
<PRINT>\" {yylval.print = c; BEGIN INITIAL; return T_PRINT;}
<PRINT>.|\n {c = strcat(c, yytext);}

<INITIAL>"/*" {BEGIN COM;}
<COM>. {;}
<COM>"*/" {BEGIN INITIAL;}
"//".*\n {;}



<INITIAL>

[[:digit:]]+ {yylval.num = atoi(yytext); return T_NUM;}
[[:alpha:]][[:alnum:]]* {yylval.id= strdup(yytext); if (is_fun(yylval.id, get_env())){return T_FUN_ID;}else
   return T_ID;}

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

[+\-*/=()\[\]\%,] {return yytext[0];}
; {return EOE;}
