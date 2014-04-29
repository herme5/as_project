
enum expr_kind {ID, FUN, APP, NUM, OP, COND, CELL, NIL, POINT, PATH, CIRCLE, BEZIER};

enum op{PLUS, MINUS, MULT, DIV, MOD, LEQ, LE, GEQ, GE, EQ, OR, AND, NOT, CONS, HEAD, TAIL, APPEND, HEADN, SETABS, SETORD, SETRAYON, SETCENTRE, SETPOINT1, SETPOINT2, SETPOINT3, SETPOINT4, ADDPATH, TRANSLATION, ROTATION, HOMOTHETIE};

struct expr;

struct fun{
  char *id;
  struct expr *body;
};

struct point{
   int abs;
   int ord;
};

struct circle{
   struct expr *centre;
   int rayon;
};

struct bezier{
   struct expr* point1;
   struct expr* point2;
   struct expr* point3;
   struct expr* point4;
};


struct cell{
  struct expr *car;
  struct expr *cdr;
};

struct app{
  struct expr *fun;
  struct expr *arg;
};

struct cond{
  struct expr *cond;
  struct expr *then_br;
  struct expr *else_br;
};

union node{
  char *id;
  struct fun fun;
  struct app app;
  enum op op;
  int num;
  struct cond cond;
  struct cell cell;
   struct point point;
   struct circle circle;
   struct bezier bezier;
};

struct expr{
  enum expr_kind type;
  union node *expr;
};


struct expr *mk_cell(struct expr * car, struct expr * cdr);
struct expr *mk_head(struct expr * list);
struct expr *mk_headn(struct expr * list, struct expr * num);
struct expr *mk_tail(struct expr * list);
struct expr *mk_node(void);
struct expr *mk_id(char *id);
struct expr *mk_fun(char *id, struct expr *body);
struct expr *mk_app(struct expr *fun, struct expr *arg);
struct expr *mk_op(enum op op);
struct expr *mk_int(int k);
struct expr *mk_cond(struct expr *cond, struct expr *then_br, struct expr *else_br);
struct expr *get_nil();
struct expr *mk_nil();
struct expr *mk_append(struct expr *list1, struct expr *list2);
struct expr *mk_reverse(struct expr *list, struct expr *list2);
struct expr *mk_point();
struct expr *mk_circle();
struct expr *mk_bezier();
struct expr *mk_path(struct expr *elem1,struct expr *elem2);
