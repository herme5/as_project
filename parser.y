%{
  /*
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
  */
#include "machine.h"
#include "js_writer.h"

#define MAX_CAR 100000

struct env *environment = NULL;
struct configuration conf_concrete;
struct configuration*conf = &conf_concrete;

struct env *get_env(){
  return environment;
}

%}

%token<num> T_NUM
%token<id> T_ID
%token<print> T_PRINT
%token EOE
%token T_LET
%token T_IF
%token T_THEN
%token T_ELSE
%token T_FUN
%token T_ARROW
%token T_IN
%token T_WHERE
%token T_DRAW

%token T_CONS
%token T_TAIL
%token T_HEAD
%token T_APPEND

%token T_EQ
%token T_DIF
%token T_LE
%token T_LEQ
%token T_GE
%token T_GEQ

%token T_AND
%token T_OR
%token T_NOT

%token T_REC

%token T_CIRCLE
%token T_BEZIER

%token T_TRANSLAT
%token T_ROTAT
%token T_HOMOT

%token T_MUSIQUEDEBUT
%token T_MUSIQUEFIN
%token T_NOTESFIN
%token<info1> T_INFO1
%token<info2> T_INFO2
%token T_SPACE
%token T_TRANS


%type<t_exp> e
%type<t_exp> f
%type<t_exp> p
%type<t_exp> l
%type<t_exp> list
%type<t_exp> path
%type<t_exp> path1
%type<t_exp> note
%type<t_exp> notel
%type<t_exp> notelist
%type<t_exp> musique

%left T_WHERE
%right T_IN
%right T_ARROW T_ELSE
%right T_IF T_THEN
%right T_AND T_OR T_NOT
%right T_EQ T_LE T_LEQ T_GE T_GEQ
%right '='
%right T_CONS
%right T_LET T_ID T_NUM
%right EOE
%left '%'
%left '+' '-'
%left '*' '/'
%nonassoc T_HEAD T_TAIL
%nonassoc NEG
%nonassoc '(' ')'
%nonassoc X_PATH

%union{
  char *id;
  char *info1;
  char *info2;
  int num;
  char *print;
  float duree;
  struct expr * t_exp;
}

%%

s :

| s EOE {;}

| s e EOE {
  conf->closure = mk_closure($2,environment);
  conf->stack = NULL;
  step(conf);
  printf(">>> "); print_expr(conf->closure->expr); printf("\n");}

| s T_LET T_REC T_ID[var] '=' e[expr] EOE {
  environment = push_rec_env($var,$expr,environment);
  struct closure * cl = mk_closure($expr,environment);
  conf->closure = cl;
  conf->stack = NULL;}

| s T_LET T_ID[var] '=' e[expr] EOE {
  struct closure * cl = mk_closure($expr,environment);
  conf->closure = cl;
  conf->stack = NULL;
  environment = push_env($var,cl,environment);}

| s T_LET T_ID[var] '=' e[expr1] e[expr2] EOE {
    struct expr *e = mk_cell(NULL,NULL);
  e = mk_app(mk_app(mk_op(CONS),$expr2),e);
  e = mk_app(mk_app(mk_op(CONS),$expr1),e);
  struct closure * cl = mk_closure(e,environment);
  conf->closure = cl;
  conf->stack = NULL;
  environment = push_env($var,cl,environment);}
;




e : e '+' e   {$$ = mk_app(mk_app(mk_op(PLUS),$1),$3);}
| e '-' e     {$$ = mk_app(mk_app(mk_op(MINUS),$1),$3);}
| e '/' e     {$$ = mk_app(mk_app(mk_op(DIV),$1),$3);}
| e '*' e     {$$ = mk_app(mk_app(mk_op(MULT),$1),$3);}
| e '%' e     {$$ = mk_app(mk_app(mk_op(MOD),$1),$3);}

| '(''-'e ')' {$$ = mk_app(mk_app(mk_op(MINUS),mk_int(0)),$3);}
| '(' e ')'   {$$ = $2;}

| T_DRAW '(' e ')' {$$ = $3;
  conf->closure = mk_closure($3,environment);
  conf->stack=NULL; step(conf);
  /*printf(">>> "); print_expr(conf->closure->expr); printf("\n")*/;
  html_write();
  js_write(draw_expr(conf->closure->expr));}

| T_PRINT '(' e ')' {$$ = $3;
  conf->closure = mk_closure($3,environment);
  conf->stack=NULL; step(conf);
  lily_write(lily_list(conf->closure->expr, 1));}

| e T_AND e   {$$ = mk_app(mk_app(mk_op(AND),$1),$3);}
| e T_OR e    {$$ = mk_app(mk_app(mk_op(OR),$1),$3);}
| T_NOT e     {$$ = mk_app(mk_op(NOT),$2);}

| e T_LE e    {$$ = mk_app(mk_app(mk_op(LE),$1),$3);}
| e T_LEQ e   {$$ = mk_app(mk_app(mk_op(LEQ),$1),$3);}
| e T_GE e    {$$ = mk_app(mk_app(mk_op(GE),$1),$3);}
| e T_GEQ e   {$$ = mk_app(mk_app(mk_op(GEQ),$1),$3);}
| e T_EQ e    {$$ = mk_app(mk_app(mk_op(EQ),$1),$3);}
| T_NUM       {$$ = mk_int($1);}

| T_IF e T_THEN e T_ELSE e {$$ = mk_cond($2,$4,$6);}
| T_ID                     {$$=mk_id($1);}

| T_FUN T_ID T_ARROW e {$$ = mk_fun ($2,$4);}

/*FONCTION AVEC PLUSIEURS PARAMETRES*/
| T_FUN T_ID p {$$ = mk_fun ($2,$3);}

| '(' e e ')'  {$$ = mk_app($2,$3);}
| f e ')'      {$$ = mk_app($1,$2);}

| T_LET T_ID[var] '=' e[expr1] T_IN e[expr2] {$$ = mk_app(mk_fun($var,$expr2),$expr1);}
| e[expr2] T_WHERE T_ID[var] '=' e[expr1]    {$$ = mk_app(mk_fun($var,$expr2),$expr1);}

| list     {$$ = $1;}
| T_HEAD e {$$ = mk_app(mk_op(HEAD),($2));}
| T_TAIL e {$$ = mk_app(mk_op(TAIL),($2));}
| e[expr] T_CONS e[list] {$$ = mk_app(mk_app(mk_op(CONS),$expr), $list);}

| '{' e ',' e '}' {
  $$ = mk_point();
  $$ = mk_app(mk_app(mk_op(SETABS),$$),$2);
  $$ = mk_app(mk_app(mk_op(SETORD),$$),$4);}

| T_CIRCLE'('e','e')' {
  $$ = mk_circle();
  $$ = mk_app(mk_app(mk_op(SETCENTRE),$$),$3);
  $$ = mk_app(mk_app(mk_op(SETRAYON),$$),$5);}

| T_BEZIER'('e','e','e','e')' {
  $$ = mk_bezier();
  $$ = mk_app(mk_app(mk_op(SETPOINT1),$$),$3);
  $$ = mk_app(mk_app(mk_op(SETPOINT2),$$),$5);
  $$ = mk_app(mk_app(mk_op(SETPOINT3),$$),$7);
  $$ = mk_app(mk_app(mk_op(SETPOINT4),$$),$9);}

| '(' path ')' {$$ = $2;}

| T_TRANSLAT '(' e ',' e ')'    {$$ = mk_app(mk_app(mk_op(TRANSLATION),$3),$5);}
| T_ROTAT '(' e ',' e ',' e ')' {$$ = mk_app(mk_app(mk_app(mk_op(ROTATION),$3),$5),$7);}
| T_HOMOT '(' e ',' e ',' e ')' {$$ = mk_app(mk_app(mk_app(mk_op(HOMOTHETIE),$3),$5),$7);}

/*MUSIQUE*/
| musique {$$ = $1;}
//| '('T_TRANS e T_ID T_ID')' {$$ = }

;

p : T_ID T_ARROW e {$$ = mk_fun ($1,$3);}
| T_ID p           {$$ = mk_fun ($1,$2);}
;

f : '(' e e {$$ = mk_app($2,$3);}
| f e       {$$ = mk_app($1,$2);}
;

list: '[' l {$$ = $2;}
;

l : e ']'   {$$ = mk_app(mk_app(mk_op(CONS),$1),mk_cell(NULL, NULL));}
| e',' l    {$$ = mk_app(mk_app(mk_op(CONS),$1),$3);}
|']'        {$$ = mk_cell(NULL, NULL);}
;

path : e '-''-' path1 {$$ = mk_app(mk_app(mk_op(ADDPATH),$1),$4);}
;

path1 : path          {$$ = mk_app(mk_op(ADDPATH),$1);}
| e                   {$$ = mk_app(mk_op(ADDPATH),$1);}
;

note : T_NUM T_INFO1 T_INFO2 T_SPACE {$$ = mk_note($1, $2, $3);}
| T_NUM T_INFO1 T_SPACE {$$ = mk_note($1, $2, "\0");}
| T_NUM T_INFO2 T_SPACE {$$ = mk_note($1, "\0", $2);}
| T_NUM T_SPACE {$$ = mk_note($1, "\0", "\0");}
| T_NUM T_INFO1 T_INFO2 T_NOTESFIN {$$ = mk_note($1, $2, $3);}
| T_NUM T_INFO1 T_NOTESFIN {$$ = mk_note($1, $2, "\0");}
| T_NUM T_INFO2 T_NOTESFIN {$$ = mk_note($1, "\0", $2);}
| T_NUM T_NOTESFIN {$$ = mk_note($1, "\0", "\0");}
;

notel : note ',' {$$ = mk_app(mk_app(mk_op(CONS),$1),mk_cell(NULL, NULL));}
| note notel {$$ = mk_app(mk_app(mk_op(CONS),$1),$2);}
;

notelist : T_MUSIQUEDEBUT notel {$$ = $2;}
;

musique : notelist T_ID ',' T_NUM T_MUSIQUEFIN {$$ = mk_musique($2, $4, 1); $$= mk_app(mk_app(mk_op(SETLIST),$$),$1);}
| notelist T_ID ',' T_NUM '/' T_NUM T_MUSIQUEFIN {$$ = mk_musique($2,$4,$6); $$= mk_app(mk_app(mk_op(SETLIST),$$),$1);}

%%

int main(int argc, char *argv[])
{
  yyparse();
  return EXIT_SUCCESS;
}
