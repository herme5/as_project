
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "machine.h"

#define MAX_CLOSURE 10000000000000000

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

void print_list(struct expr *list){
  printf("{");
  struct expr *tmp = list;
  if (tmp != NULL){
    while(tmp->expr->cell.cdr != NULL){
      print_expr(tmp->expr->cell.car);
      tmp = (tmp->expr->cell.cdr);
      if (tmp->expr->cell.car != NULL){
	printf(", ");
      }
    }
  }
  printf("}");
}

void print_expr(struct expr *expr){
  switch(expr->type){
  case CELL: print_list(expr);return;
  case NUM: printf("%d",expr->expr->num);return;
  case FUN: printf("fun(%s)->",expr->expr->fun.id);print_expr(expr->expr->fun.body);return;
  case ID : printf("%s",expr->expr->id);return;
  case APP : print_app(expr);return;
  case OP : print_op(expr);return;
  case COND : print_cond(expr);return;
  case NIL : printf ("NIL");return;
  case POINT : print_point(expr);return;
  case CIRCLE : print_circle(expr);return;
  case BEZIER : print_bezier(expr);return;
  default : printf("non reconnu");
  }
}

void print_bezier(struct expr *bezier){
   printf("(");
   print_expr(bezier->expr->bezier.point1);
   printf(",");
   print_expr(bezier->expr->bezier.point2);
   printf(",");
   print_expr(bezier->expr->bezier.point3);
   printf(",");
   print_expr(bezier->expr->bezier.point4);
   printf(")");
}

void print_circle(struct expr *circle){
   printf("(");
   print_expr(circle->expr->circle.centre);
   printf(",%d)", circle->expr->circle.rayon);
}

void print_point(struct expr *point){
   printf("(%d,%d)",point->expr->point.abs,point->expr->point.ord);
}

void print_cond(struct expr *cond){
   printf("\nif (");
   print_expr(cond->expr->cond.cond);
   printf(")\nthen ");
   print_expr(cond->expr->cond.then_br);
   printf("\nelse ");
   print_expr(cond->expr->cond.else_br);

}

void print_app(struct expr *app){
  if (app->expr->app.fun->type == OP){
     print_expr(app->expr->app.arg);printf(" ");print_expr(app->expr->app.fun);
  }
  else{
    print_expr(app->expr->app.fun);printf(" ");print_expr(app->expr->app.arg);
  }
}


void print_op(struct expr *op){
  switch(op->expr->op){
  case PLUS: printf("+");return;
  case MINUS: printf("-");return;
  case MULT: printf("*");return;
  case DIV: printf("/");return;
  case MOD: printf("mod");return;
  case LEQ: printf("<=");return;
  case LE: printf("<");return;
  case GEQ: printf(">=");return;
  case GE: printf(">");return;
  case EQ: printf("==");return;
  case OR: printf("||");return;
  case AND: printf("&&");return;
  case NOT: printf("!");return;
  case CONS: printf("cons");return;
  case HEAD: printf("head");return;
  case TAIL: printf("tail");return;
  }
}

int list_equal(struct expr* l1, struct expr* l2){
   if (l1 == NULL && l2 == NULL){
      return 1;
   }
   if (l1 == NULL || l2 == NULL){
      return 0;
   }
   if (l1->expr->cell.car == NULL && l2->expr->cell.car == NULL){
      return 1;
   }
   if (l1->expr->cell.car == NULL || l2->expr->cell.car == NULL){
      return 0;
   }
   if (element_equal(l1->expr->cell.car,l2->expr->cell.car)){
      return list_equal(l1->expr->cell.cdr, l2->expr->cell.cdr);
   }
   return 0;
}

int element_equal(struct expr* e1, struct expr* e2){
   assert (e1->type == e2->type);
   assert (e1->type == NUM || e1->type == CELL);
   switch (e1->type){
      case NUM: return e1->expr->num == e2->expr->num;
      case CELL: return list_equal(e1,e2);
      default : return 0;
   }
}

int get_num(struct configuration * conf){
  step(conf);
  assert(conf->closure->expr->type==NUM);
  return conf->closure->expr->expr->num;
}

struct cell get_cell(struct configuration * conf){
  step(conf);
  assert(conf->closure->expr->type==CELL);
  return conf->closure->expr->expr->cell;
}

struct expr* set_abs(struct expr* point,int abs){
   point->expr->point.abs = abs;
   return point;
}

struct expr* set_ord(struct expr* point,int ord){
   point->expr->point.ord = ord;
   return point;
}

struct expr* set_rayon(struct expr* cercle,int rayon){
   cercle->expr->circle.rayon = rayon;
   return cercle;
}
struct expr* set_centre(struct expr* cercle,struct expr *point){
   cercle->expr->circle.centre = point;
   return cercle;
}

struct expr* set_point(struct expr* bezier,struct expr *point,int pos){
   switch (pos){
      case 1: bezier->expr->bezier.point1 = point; break;
      case 2: bezier->expr->bezier.point2 = point; break;
      case 3: bezier->expr->bezier.point3 = point; break;
      case 4: bezier->expr->bezier.point4 = point; break;
   }
   return bezier;
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
  case POINT:
    return;
  case CIRCLE:
    return;
  case BEZIER:
    return;
  case CELL:
    return;
  case NIL:
    return;
  case OP:
    {
      //printf("1\n");
      struct stack *stack = conf->stack;

      if(stack == NULL){return;}
      //printf("2\n");
      struct closure *arg1 = stack->closure;
      stack = pop_stack(stack);
      conf->closure = arg1;
      conf->stack = NULL;
      step(conf);
      //printf("3\n");
      int k1, k2;
      struct expr * e1 = conf->closure->expr;
      //printf("e = %d\n", e1->expr->num);
      struct expr * c1,* c2;
      if(conf->closure->expr->type==NUM){
	k1 = get_num(conf);
	switch(expr->expr->op){
	case NOT: conf->closure->expr->expr->num = !k1; return;
	default: ;
	}
      }
      if(conf->closure->expr->type==POINT){
         c1 = conf->closure->expr;
      }
      if(conf->closure->expr->type==CIRCLE){
         c1 = conf->closure->expr;
      }
      if(conf->closure->expr->type==BEZIER){
         c1 = conf->closure->expr;
      }
      if(conf->closure->expr->type==CELL){
	//printf("5\n");
	c1 = conf->closure->expr;
	switch(expr->expr->op){
	case HEAD: conf->closure = mk_closure(mk_head(conf->closure->expr),NULL); return;
	case TAIL: conf->closure = mk_closure(mk_tail(conf->closure->expr),NULL); return;
	default: ;
	}
      }

      if(stack == NULL){return;}
      //printf("6\n");
      arg1=conf->closure;
      struct closure *arg2 = stack->closure;
      stack = pop_stack(stack);
      conf->closure = arg2;
      conf->stack = NULL;
      step(conf);
      //printf("7\n");
      if(conf->closure->expr->type==NUM){
         //printf("8\n");
	k2 = get_num(conf);
	switch (expr->expr->op){
    case SETABS: conf->closure = mk_closure(set_abs(c1,k2),NULL); return;

    case SETORD: conf->closure = mk_closure(set_ord(c1,k2),NULL); return;

    case SETRAYON: conf->closure = mk_closure(set_rayon(c1,k2),NULL);return;

    case PLUS:  conf->closure = mk_closure(mk_int(k1 + k2 ),NULL); return;
	case MINUS: conf->closure = mk_closure(mk_int(k1 - k2 ),NULL); return;
	case MULT:  conf->closure = mk_closure(mk_int(k1 * k2 ),NULL); return;
	case DIV:   assert(k2!=0);
	            conf->closure = mk_closure(mk_int(k1 /  k2),NULL); return;
	case MOD:   conf->closure = mk_closure(mk_int(k1 %  k2),NULL); return;
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
      if(conf->closure->expr->type==POINT){
         c2 = conf->closure->expr;
         switch (expr->expr->op){
            case SETCENTRE: conf->closure = mk_closure(set_centre(c1,c2),NULL); return;
            case SETPOINT1: conf->closure = mk_closure(set_point(c1,c2,1),NULL); return;
            case SETPOINT2: conf->closure = mk_closure(set_point(c1,c2,2),NULL); return;
            case SETPOINT3: conf->closure = mk_closure(set_point(c1,c2,3),NULL); return;
            case SETPOINT4: conf->closure = mk_closure(set_point(c1,c2,4),NULL); return;
            default: ;
         }

      }
      if(conf->closure->expr->type==CELL){
	//printf("9\n");
	c2 = conf->closure->expr;
	//printf("10\n");

	//print_list(conf->closure->expr);

	switch (expr->expr->op){
	case CONS:  conf->closure = mk_closure(mk_cell(e1,conf->closure->expr),NULL); return;
	case APPEND: conf->closure = mk_closure(mk_append(c1,c2),NULL); return;
	case HEADN: conf->closure = mk_closure(mk_headn(c2, e1),NULL); return;
       case EQ: conf->closure = mk_closure(mk_int(list_equal(c1,c2)),NULL); return;
	default:    assert(0);
	}
      }
      //printf("10\n");
    }
    ;
  default: assert(0);
  }
}
