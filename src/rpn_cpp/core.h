#ifndef SRC_RPR_CORE_H_
#define SRC_RPR_CORE_H_
#include <math.h>

#include "expr.h"
#ifndef MAXSTR
#define MAXSTR 10000
#endif
double rpn_reduce(const struct expr *e, double x);
double calc(char *string, float x, int *good);

#endif  // SRC_RPR_CORE_H_
