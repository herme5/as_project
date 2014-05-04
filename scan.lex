%{
#include "parser.tab.h"

  char *c;

%}

%option noyywrap

%x COM
%x NOTES
%x NOTESNEG
%x MUSIQUE
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

Bezier {return T_BEZIER;}
bezier {return T_BEZIER;}

Translation {return T_TRANSLAT;}
translation {return T_TRANSLAT;}

Rotation {return T_ROTAT;}
rotation {return T_ROTAT;}

Homothetie {return T_HOMOT;}
homothetie {return T_HOMOT;}

draw {return T_DRAW;}
print {return T_PRINT;}

<INITIAL>"/*"     {BEGIN COM;}
<INITIAL>"//".*\n {;}

<COM>.      {;}
<COM>"*/"   {BEGIN INITIAL;}

<INITIAL>"{(" {BEGIN NOTES; return T_MUSIQUEDEBUT;}

<NOTES>[[:digit:]]+ {yylval.num = atoi(yytext); return T_NUM;}
<NOTES>"(-" {BEGIN NOTESNEG;}
<NOTES>[#b] {yylval.id= strdup(yytext); return T_INFO1;}
<NOTES>[-.]+ {yylval.id= strdup(yytext); return T_INFO2;}
<NOTES>"S" {yylval.num = 0; return T_NUM;}
<NOTES>[[:space:]] {return T_SPACE;}
<NOTES>")" {BEGIN MUSIQUE; return T_NOTESFIN;}

<NOTESNEG>[[:digit:]]+ {yylval.num = -atoi(yytext); return T_NUM;}
<NOTESNEG>")" {BEGIN NOTES;}

<MUSIQUE>[,/] {return yytext[0];}
<MUSIQUE>[[:alpha:]](_|#|[[:alnum:]])* {yylval.id= strdup(yytext); return T_ID;}
<MUSIQUE>[[:digit:]]+ {yylval.num = atoi(yytext); return T_NUM;}
<MUSIQUE>"}" {BEGIN INITIAL; return T_MUSIQUEFIN;}
<MUSIQUE>[[:space:]] {;}

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
