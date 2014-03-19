%{

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


//------------------------------------------
//--Fonctions de gestion de lenvironnement--
//------------------------------------------


typedef struct var_t * var;

struct var_t{
  char *id;
  int val;
  var next;
};

var env = NULL;


var find_var(char *id, var e)
{
  if(e == NULL){
    return NULL;
  }

  if (!strcmp(id, e->id))
    return e;
  else
    return find_var(id,e->next);
}

int get_var(char *id, var e)
{
  var tmp = find_var(id, e);
  if (tmp == NULL){
    return 0;
  }
  return tmp->val;
}

var add_var(char *id, int k, var e)
{
  var new = malloc(sizeof(*new));
  new->id = id;
  new->val = k;
  new->next = e;
  return new;
}

var push_var(char *id, int k, var e)
{
  var tmp = find_var(id, e);
  if (tmp == NULL){
    return add_var(id, k, e);
  }
  else {
    tmp->val = k;
    return tmp;
  }
}

//---------------------------------------

%}

%token<num> NB
%token<id> ID
%token FIN_EXP
%token POW

%token EQ
%token DIF
%token LS
%token LEQ
%token GR
%token GEQ

%token AND
%token OR
%token NOT

%type<num> e

%right '?' ':'
%right AND OR NOT
%right EQ DIF LS LEQ GR GEQ
%right '='
%left '+' '-'
%left '*' '/'
%right '%'
%nonassoc NEG
%right POW


%union{
  char *id;
  int num;
}

%%

s :
  | s e FIN_EXP {printf(">>> %d\n", $2);}
  ;

e : e '+' e       {$$ = $1 + $3;}
  | e '-' e       {$$ = $1 - $3;}
  | e '/' e       {$$ = $1 / $3;}
  | e '*' e       {$$ = $1 * $3;}
  | e '%' e       {$$ = $1 % $3;}

  | e POW e       {$$ = power($1, $3);}
  | '(' e ')'     {$$ = $2;}

  | e AND e  {$$ = $1 && $3;}
  | e OR e   {$$ = $1 || $3;}
  | NOT e    {$$ = !$2;}

  | e LS e {if($1 < $3){$$ = 1;}else{$$ = 0;}}
  | e LEQ e {if($1 <= $3){$$ = 1;}else{$$ = 0;}}
  | e GR e {if($1 > $3){$$ = 1;}else{$$ = 0;}}
  | e GEQ e {if($1 >= $3){$$ = 1;}else{$$ = 0;}}
  | e EQ e {if($1 == $3){$$ = 1;}else{$$ = 0;}}
  | e DIF e {if($1 != $3){$$ = 1;}else{$$ = 0;}}

  | e '?' e ':' e {if($1){$$ = $3;}else{$$ = $5;}}

  | ID '=' e        {env = push_var($1, $3, env); $$ = $3;}
  | ID              {$$ = get_var($1, env);}
  | NB              {$$ = $1;}
  | '-' e %prec NEG {$$ = -$2;}
  ;

%%

int main(int argc, char *argv[])
{
  yyparse();
  return EXIT_SUCCESS;
}
