%{

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "machine.h"

struct env * environment = NULL;
struct configuration conf_concrete;
struct configuration * conf = &conf_concrete;

%}

%token<num> T_NUM
%token<id> T_ID
%token EOE
%token T_LET
%token T_IF
%token T_THEN
%token T_ELSE
%token T_FUN
%token T_ARROW
%token T_IN
%token T_WHERE

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

%type<t_exp> e
%type<t_exp> p
%type<t_exp> f
%type<t_exp> l
%type<t_exp> list

%left T_WHERE
%right T_IN
%right T_ARROW T_ELSE
%right T_AND T_OR T_NOT
%right T_EQ T_LE T_LEQ T_GE T_GEQ
%right '='
%right T_CONS
%right T_LET T_ID T_NUM
%right EOE
%nonassoc NEG
%left '%'
%left '+' '-'
%left '*' '/'

%union{
  char *id;
  int num;
  struct expr * t_exp;
}

%%


s :
| s e EOE {
  conf->closure = mk_closure($2,environment);
  conf->stack=NULL;
  step(conf);
  switch(conf->closure->expr->type){
  case NUM:  printf(">>> %d \n", conf->closure->expr->expr->num); break;
  case CELL: printf(">>> "); print_list((conf->closure->expr)); printf("\n"); break;
  default:   break;
  }
}

| s T_LET T_ID[var] '=' e[expr] EOE {
  environment = push_rec_env($var,$expr,environment);
  conf->closure = mk_closure($expr,environment);
  conf->stack=NULL;
  step(conf);
  switch(conf->closure->expr->type){
  case NUM:  printf(">>> %d \n", conf->closure->expr->expr->num); break;
  case CELL: printf(">>> "); print_list((conf->closure->expr)); printf("\n"); break;
  default:   break;
  }
}
;


e : e '+' e   {$$ = mk_app(mk_app(mk_op(PLUS),$1),$3);}
| e '-' e     {$$ = mk_app(mk_app(mk_op(MINUS),$1),$3);}
| e '/' e     {$$ = mk_app(mk_app(mk_op(DIV),$1),$3);}
| e '*' e     {$$ = mk_app(mk_app(mk_op(MULT),$1),$3);}
| e '%' e     {$$ = mk_app(mk_app(mk_op(MOD),$1),$3);}

| '(' '-' e ')' {$$ = mk_app(mk_app(mk_op(MINUS),mk_int(0)),$3);}
| '(' e ')'     {$$ = $2;}

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
| T_FUN T_ID T_ARROW e     {$$ = mk_fun($2,$4);}
| '(' e e ')'              {$$ = mk_app($2,$3);}
| f e ')'                  {$$ = mk_app($1,$2);}
| T_ID                     {$$=mk_id($1);}

/*FONCTION AVEC PLUSIEURS PARAMETRES*/
| T_FUN T_ID p {$$ = mk_fun ($2,$3);}

/* LET IN */
| T_LET T_ID[var] '=' e[expr1] T_IN e[expr2] {$$ = mk_app(mk_fun($var,$expr2),$expr1);}

/* WHERE */
| e[expr2] T_WHERE T_ID[var] '=' e[expr1] {$$ = mk_app(mk_fun($var,$expr2),$expr1);}

| T_CONS e[expr] e[list] {$$ = mk_app(mk_app(mk_op(CONS),$expr), $list);}
| list {$$=$1;}
//TODO | T_HEAD e {$$=mk_head($2);}
//TODO | T_TAIL e {$$=mk_tail($2);}
//TODO | T_APPEND e e {}
;

/* | T_LET T_ID '=' e { */
/*   printf("affectation ici\n"); */
/*   environment = push_rec_env($2,$4,environment); */
/*   conf->closure = mk_closure($4,environment); */
/*   conf->stack=NULL; */
/*   step(conf); */
/*   $$ = $4;} */
/* | TFUN TID[1] TID[2] ARROW e -> TFUN TID[1] ARROW mkfun(TID[2], e)*/

p : T_ID T_ARROW e {$$ = mk_fun ($1,$3);}
| T_ID p           {$$ = mk_fun ($1, $2);}
;

f : '(' e e {$$ = mk_app($2, $3);}
| f e       {$$ = mk_app($1,$2);}
;

list: '[' l {$$ = $2;}

l :
e ']'       {$$ = mk_app(mk_app(mk_op(CONS),$1),mk_cell(NULL, NULL));}
| e',' l    {$$ = mk_app(mk_app(mk_op(CONS),$1),$3);}
|']'        {$$ = mk_cell(NULL, NULL);}


%%

int main(int argc, char *argv[])
{
  yyparse();
  return EXIT_SUCCESS;
}
