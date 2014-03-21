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

%token<num> TNUM
%token<id> TID
%token FIN_EXPR
%token LET
%token IF
%token THEN
%token ELSE
%token TFUN
%token ARROW

%token TEQ
%token TDIF
%token TLS
%token TLEQ
%token TGR
%token TGEQ

%token TAND
%token TOR
%token TNOT

%type<exxp> e

%right ARROW ELSE
%right TAND TOR TNOT
%right TEQ TLE TLEQ TGE TGEQ
%right '='
%right LET TID TNUM
%left '+' '-'
%left '*' '/'
/* %nonassoc NEG */


%union{
  char *id;
  int num;
  struct expr * exxp;
}

%%


s :
| s e FIN_EXPR {conf->closure = mk_closure($2,environment);
   conf->stack=NULL;
   step(conf);
   if(conf->closure->expr->type==NUM)
     printf("Valeur : %d \n", conf->closure->expr->expr->num);
 }
| s LET TID[var] '=' e[expr] FIN_EXPR {environment = push_rec_env($var,$expr,environment);
      conf->closure = mk_closure($expr,environment);
   conf->stack=NULL;
   step(conf);
   if(conf->closure->expr->type==NUM)
     printf("Valeur de %s : %d \n",$var, conf->closure->expr->expr->num);}
;


e : e '+' e       {$$ = mk_app(mk_app(mk_op(PLUS),$1),$3);}
| e '-' e       {$$ = mk_app(mk_app(mk_op(MINUS),$1),$3);}
| e '/' e       {$$ = mk_app(mk_app(mk_op(DIV),$1),$3);}
| e '*' e       {$$ = mk_app(mk_app(mk_op(MULT),$1),$3);}

| '(' e ')'     {$$ = $2;}

| e TAND e  {$$ = mk_app(mk_app(mk_op(AND),$1),$3);}
| e TOR e   {$$ = mk_app(mk_app(mk_op(OR),$1),$3);}
| TNOT e    {$$ = mk_app(mk_op(NOT),$2);}

| e TLE e {$$ = mk_app(mk_app(mk_op(LE),$1),$3);}
| e TLEQ e {$$ = mk_app(mk_app(mk_op(LEQ),$1),$3);}
| e TGE e {$$ = mk_app(mk_app(mk_op(GE),$1),$3);}
| e TGEQ e {$$ = mk_app(mk_app(mk_op(GEQ),$1),$3);}
| e TEQ e {$$ = mk_app(mk_app(mk_op(EQ),$1),$3);}
| TNUM {$$ = mk_int($1);}
/* | '-' TNUM %prec NEG {$$ = mk_int(-$2);} */

| IF e THEN e ELSE e {$$ = mk_cond($2,$4,$6);}
| TFUN TID ARROW e {$$ = mk_fun($2,$4);}
| '(' e e ')' {$$ = mk_app($2,$3);}
| TID {$$=mk_id($1);}

;


%%

int main(int argc, char *argv[])
{
  yyparse();
  return EXIT_SUCCESS;
}
