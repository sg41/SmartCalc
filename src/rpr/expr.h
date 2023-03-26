#ifndef SRC_RPR_EXPR_H_
#define SRC_RPR_EXPR_H_

// Functions
#define sin_FUNCTION 's'
#define cos_FUNCTION 'c'
#define tan_FUNCTION 't'
#define ctg_FUNCTION 'g'
#define abs_FUNCTION 'a'
#define log_FUNCTION 'l'
#define ln_FUNCTION 'e'
#define sqrt_FUNCTION 'q'
#define asin_FUNCTION 'i'
#define acos_FUNCTION 'o'
#define atan_FUNCTION 'n'
#define error_FUNCTION 'E'
// Operators:
#define mod_FUNCTION '%'
#define UNARY_PLUS 'p'
#define UNARY_MINUS 'm'

#define OPERAND 0
#define OPERATOR 1
#define UNARYOPERATOR 2
#define VARIABLE 3
#define L_BRACKET 4
#define R_BRACKET 5
#define FUNCTION 6
#define ERROR 7

#define ADD_SCORE 2
#define SUB_SCORE 2
#define MULT_SCORE 4
#define DIV_SCORE 4
#define FUNC_SCORE 10
#define EXP_SCORE 6
#define U_SCORE 8
#define L_SCORE 12
#define R_SCORE 12

struct expr {
  unsigned int length;
  struct ll_node *head;
};

struct expr *expr_new(void);
void expr_destroy(struct expr **e);
void expr_add_symbol(struct expr *e, const unsigned int s, const double d);
void expr_print(const struct expr *e);
struct expr *expr_shunt(const struct expr *e);
struct expr *expr_from_array(const char *a);
char *one_expr_from_string(char *prog, struct expr **infix_to_fill, int *good,
                           int *parents);
struct expr *expr_from_string(char *a, int *good);
int is_alpha(char c);
int is_digit(char c);
int is_space(char c);

#endif  // SRC_RPR_EXPR_H_
