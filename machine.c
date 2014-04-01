
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "machine.h"

#define MAX_CLOSURE 1000000

int nb_closure;

struct closure *mk_closure(struct expr *expr, struct env *env){
  assert(nb_closure<MAX_CLOSURE);
  struct closure *cl = malloc(sizeof(struct closure));
  cl->expr = expr;
  cl->env = env;
  nb_closure++;
  return cl;
}

struct configuration *mk_conf(struct closure *cl){
  struct configuration *conf = malloc(sizeof(struct configuration));
  conf->closure = cl;
  conf->stack=NULL;
  return conf;
}

void print_env(const struct env *env){
  if(env==NULL){printf(" #\n");}
  else{printf(" %s ",env->id);print_env(env->next);}
}

struct env *push_env(char *id, struct closure *cl, struct env *env){
  struct env *e = malloc(sizeof(struct env));
  e->id = id;
  e->closure = cl;
  e->next = env;
  //print_env(e);
  return e;
}

struct env *push_rec_env(char *id, struct expr *expr, struct env *env){
  struct env *e = malloc(sizeof(struct env));
  struct closure *cl = mk_closure(expr,e);
  e-> id = id;
  e->closure = cl;
  e->next = env;
  //print_env(e);
  return e;
}

struct closure *search_env(char *id, struct env *env){
  assert(env!=NULL);
  if(strcmp(id,env->id)==0){return env->closure;}
  else{return search_env(id,env->next);}  
}

struct stack *pop_stack(struct stack *stack){
  struct stack *next = stack->next;
  free(stack);
  return next;
}

struct stack *push_stack(struct closure *cl, struct stack *stack){
  struct stack *st = malloc(sizeof(struct stack));
  st->closure = cl;
  st->next = stack;
  return st;
}

void print_list(struct cell *list){
  printf("{");
  struct cell *tmp = list;
  if (tmp != NULL){
    while(tmp->next != NULL){printf("%d ;",list->e->expr->num); tmp = tmp->next;}
    printf("%d}");
  }
}

int get_num(struct configuration * conf){
  step(conf);
  assert(conf->closure->expr->type==NUM);
  return conf->closure->expr->expr->num;
}

struct cell *get_list(struct configuration * conf){
  step(conf);
  assert(conf->closure->expr->type==LIST);
  return conf->closure->expr->expr->cell;
}

void step(struct configuration *conf){
  struct expr *expr = conf->closure->expr;
  struct env *env = conf->closure->env;
  struct stack *stack = conf->stack;
  assert(expr!=NULL);
  switch (expr->type){
  case FUN: 
    {// printf("fun\n");
      if(stack==NULL){return;}
      char *var = expr->expr->fun.id;
      struct expr *body = expr->expr->fun.body;
      struct env *e = push_env(var, stack->closure,env);
      conf->closure=mk_closure(body,e);
      conf->stack = pop_stack(stack);
      return step(conf);
    }
  case APP: 
    { 
      struct expr *fun = expr->expr->app.fun;
      struct expr *arg = expr->expr->app.arg;
      conf->closure = mk_closure(fun,env);
      conf->stack = push_stack(mk_closure(arg,env),conf->stack);
      return step(conf);
    }
  case ID: //printf("id: %s\n", expr->expr->id);
    assert(env!=NULL);
    conf->closure = search_env(expr->expr->id,env);
    return step(conf);
  case COND:
    { 
      struct stack *stack = conf->stack;
      struct closure *cl_cond = mk_closure(expr->expr->cond.cond,env);
      conf->closure = cl_cond;
      conf->stack=NULL;
      int k = get_num(conf);
      conf->stack=stack;
      if(k==0){
        //printf("cond false\n");
        conf->closure = mk_closure(expr->expr->cond.else_br,env);
      }
      else{
        //printf("cond true\n");
        conf->closure = mk_closure(expr->expr->cond.then_br,env);
      }
      return step(conf);
    }
  case NUM: 
    return;
  case LIST:
    return;
  case OP: 
    {
      struct stack *stack = conf->stack;
      
      if(stack == NULL){return;}
      struct closure *arg1 = stack->closure;
      stack = pop_stack(stack);
      conf->closure = arg1;
      conf->stack = NULL;
      step(conf);
      int k1, k2;
      struct cell *c1, *c2;
      if(conf->closure->expr->type==NUM){
	k1 = get_num(conf);
	switch(expr->expr->op){
	case NOT: conf->closure->expr->expr->num = !k1; return;
	default: ;
	}
      }
      if(conf->closure->expr->type==LIST){
	c1 = get_list(conf);
	switch(expr->expr->op){
	case HEAD: conf->closure->expr->expr->cell = c1; return;
	case TAIL: conf->closure->expr->expr->cell = c1; return;
	default: ; 
	}
      }
      
      if(stack == NULL){return;}
      arg1=conf->closure;
      struct closure *arg2 = stack->closure;
      stack = pop_stack(stack);
      conf->closure = arg2;
      conf->stack = NULL;
      step(conf);
      if(conf->closure->expr->type==NUM){
	k2 = get_num(conf);
	switch (expr->expr->op){
	case PLUS:  conf->closure = mk_closure(mk_int(k1 + k2 ),NULL); return;
	case MINUS: conf->closure = mk_closure(mk_int(k1 - k2 ),NULL); return;
	case MULT:  conf->closure = mk_closure(mk_int(k1 * k2 ),NULL); return;
	case DIV:   assert(k2!=0);
	            conf->closure = mk_closure(mk_int(k1 /  k2),NULL); return;
	case LEQ:   conf->closure = mk_closure(mk_int(k1 <= k2),NULL); return;
	case LE:    conf->closure = mk_closure(mk_int(k1 <  k2),NULL); return;
	case GEQ:   conf->closure = mk_closure(mk_int(k1 >= k2),NULL); return;
	case GE:    conf->closure = mk_closure(mk_int(k1 >  k2),NULL); return;
	case EQ:    conf->closure = mk_closure(mk_int(k1 == k2),NULL); return;
	case OR:    conf->closure = mk_closure(mk_int(k1 || k2),NULL); return;
	case AND:   conf->closure = mk_closure(mk_int(k1 && k2),NULL); return;
	default:    ;
	}
      }
      if(conf->closure->expr->type==LIST){
	c2 = get_list(conf);
	switch (expr->expr->op){
	case CONS:  conf->closure = mk_closure(mk_cons(conf->closure->expr,c2),NULL); return;
	default:    assert(0);
	}
      }
    }
    ;
  default: assert(0);
  }
}
