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
rec   {return T_REC;}

::    {return T_CONS;}
hd    {return T_HEAD;}
head  {return T_HEAD;}
pop   {return T_HEAD;}
tail  {return T_TAIL;}
tl    {return T_TAIL;}

Cercle {return T_CIRCLE;}
cercle {return T_CIRCLE;}

bezier {return T_BEZIER;}
Bezier {return T_BEZIER;}

Translation {return T_TRANSLAT;}
translation {return T_TRANSLAT;}

Rotation {return T_ROTAT;}
rotation {return T_ROTAT;}

Homothetie {return T_HOMOT;}
homothetie {return T_HOMOT;}

<INITIAL>print[[:space:]]*\" {c = malloc(10000*sizeof(char)); BEGIN PRINT;}

<INITIAL>"/*"     {BEGIN COM;}
<INITIAL>"//".*\n {;}

<PRINT>\"   {yylval.print = c; BEGIN INITIAL; return T_PRINT;}
<PRINT>.|\n {c = strcat(c, yytext);}

<COM>.      {;}
<COM>"*/"   {BEGIN INITIAL;}

[[:digit:]]+ {yylval.num = atoi(yytext); return T_NUM;}
[[:alpha:]](_|[[:alnum:]])* {yylval.id= strdup(yytext); return T_ID;}

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

[+\-*/=()\[\]\%,\{\}] {return yytext[0];}

; {return EOE;}
