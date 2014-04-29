
#include <stdlib.h>
#include "expr.h"
#include <stdio.h>

struct expr *mk_node(void){
  struct expr *e = malloc(sizeof(struct expr));
  e->expr = malloc(sizeof(union node));
  return e;
}

struct expr *mk_id(char *id){
  struct expr *e = mk_node();
  e->type = ID;
  e->expr->id = id;
  return e;
}

struct expr *mk_fun(char *id, struct expr *body){
  struct expr *e = mk_node();
  e->type = FUN;
  e->expr->fun.id = id;
  e->expr->fun.body = body;
  return e;
}

struct expr *mk_app(struct expr *fun, struct expr *arg){
  struct expr *e = mk_node();
  e->type = APP;
  e->expr->app.fun=fun;
  e->expr->app.arg=arg;
  return e;
}

struct expr *mk_op(enum op op){
  struct expr *e = mk_node();
  e->type=OP;
  e->expr->op = op;
  return e;
}

struct expr *mk_int(int k){
  struct expr *e = mk_node();
  e->type=NUM;
  e->expr->num = k;
  return e;
}

struct expr *mk_cond(struct expr *cond, struct expr *then_br, struct expr *else_br){
  struct expr *e = mk_node();
  e->type = COND;
  e->expr->cond.cond = cond;
  e->expr->cond.then_br = then_br;
  e->expr->cond.else_br = else_br;
  return e;
}

struct expr *mk_circle(){
   struct expr *e = mk_node();
   e->type=CIRCLE;
   return e;
}

struct expr *mk_bezier(){
   struct expr *e = mk_node();
   e->type=BEZIER;
   return e;
}

struct expr *mk_point(){
   struct expr *e = mk_node();
   e->type=POINT;
   return e;
}

struct expr *mk_cell(struct expr *car, struct expr *cdr){
  struct expr *res = mk_node();
  res->type = CELL;
  res->expr->cell.car = car;
  res->expr->cell.cdr = cdr;
  return res;
}

struct expr *mk_path(struct expr *car, struct expr *cdr){
   struct expr *res = mk_node();
   res->type = PATH;
   res->expr->cell.car=car;
   res->expr->cell.cdr=cdr;
   return res;
}

struct expr *mk_head(struct expr *list){
  struct expr *res = mk_node();
  if (list->expr->cell.car == NULL){
    return mk_nil();
  }
  res = list->expr->cell.car;
  return res;
}

struct expr *mk_headn(struct expr * list, struct expr * num){
  struct expr *templist = list;
  int nb = num->expr->num;
  while (nb > 0){
    templist = mk_tail(templist);
    nb--;
  }
  return mk_head(templist);
}

struct expr *mk_tail(struct expr *list){
  struct expr *res = mk_node();
  if (list->expr->cell.cdr == NULL){
    return mk_cell(NULL,NULL);
  }
  res = list->expr->cell.cdr;
  return res;
}

struct expr *mk_nil(){
  struct expr *res = mk_node();
  res->type= NIL;
  return res;
}

struct expr *mk_append(struct expr *list1, struct expr *list2){
  struct expr *revlist1 = mk_reverse(list1, mk_cell(NULL,NULL));
  return mk_reverse(revlist1, list2);
}

struct expr *mk_reverse(struct expr *list, struct expr *finallist){
  if (list->type == NIL || list->expr->cell.cdr == NULL){
    return finallist;
  }
  else{
    return (mk_reverse(mk_tail(list),mk_cell(mk_head(list),finallist)));
  }
}

//fold f [a0, a1, ..., an] b
//(f an (...(f a2(f a1 a0)) ...))
