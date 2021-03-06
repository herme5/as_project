
#include "expr.h"

struct env;
struct closure;
struct configuration;
struct stack;

struct env{
  char *id;
  struct closure *closure;
  struct env *next;
};

struct closure{
  struct expr *expr;
  struct env *env;
};

struct configuration{
  struct closure *closure;
  struct stack *stack;
};

struct stack{
  struct closure *closure;
  struct stack *next;
};

struct closure *mk_closure(struct expr *expr, struct env *env);
struct configuration *mk_conf(struct closure *cl);
struct env *push_env(char *id, struct closure *cl, struct env *env);
struct env *push_rec_env(char *id, struct expr *expr, struct env *env);
void step(struct configuration *conf);

void print_list  (struct expr *list);
void print_expr  (struct expr *expr);
void print_op    (struct expr *op);
void print_app   (struct expr *app);
void print_cond  (struct expr *cond);
void print_point (struct expr *point);
void print_circle(struct expr *circle);
void print_bezier(struct expr *bezier);
void print_path  (struct expr *path);
void print_musique_liste(struct expr *liste);
void print_musique(struct expr *musique);
void print_note(struct expr *note);
char *draw_expr(struct expr *form);
char *lily_list(struct expr *list, int first);
char *lily(struct expr *musique);
char *get_note(struct expr* note, char *tonique, int dureenum, int dureeden, int octave);


int element_equal(struct expr *e1, struct expr *e2);
float to_radian(int angle);

struct expr *set_abs   (struct expr *point, int abs);
struct expr *set_ord   (struct expr *point, int ord);
struct expr *set_rayon (struct expr *cercle, int rayon);
struct expr *set_centre(struct expr *cercle, struct expr *point);
struct expr *set_point (struct expr *bezier, struct expr *point, int pos);
struct expr *set_list (struct expr *musique, struct expr *list);

struct expr *translation(struct expr *elem, struct expr *vecteur);
struct expr *rotation   (struct expr *elem, struct expr *centre, struct expr *angle);
struct expr *homothetie (struct expr *elem, struct expr *centre, struct expr *ratio);
struct expr *transposition (struct expr *musique, char* tonique1, char* tonique2);
struct expr *addtomusic (struct expr *musique, int add);
struct expr *multtomusic (struct expr *musique, int mult);
struct expr *divtomusic (struct expr *musique, int div);
struct expr *invmusic (struct expr *musique);
