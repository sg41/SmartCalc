#include "graph.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rpr/core.h"
#include "rpr/llist.h"

#ifdef TXT_VERSION
int main(int argc, char *argv[]) {
#else
int txt_main(int argc, char *argv[]) {
#endif
  struct expr *infix;
  struct expr *postfix;
  char string[MAXSTR + 1] = {0};

  if (argc > 1) {
    for (int i = 1; i < argc; i++) strncat(string, argv[i], MAXSTR);
  } else {
    input(string);
  }

  if (strlen(string)) {
    int good = 1;
    infix = expr_from_string(string, &good);
    if (good) {
      postfix = expr_shunt(infix);
      draw_txt_graph(rpn_reduce, postfix);
      expr_destroy(&postfix);
    }
    expr_destroy(&infix);
  }
  return 0;
}

char *no_space(char *strin) {
  int i, j;
  char *dest = strin;
  for (i = j = 0; strin[i] != '\0'; i++)
    if (strin[i] != ' ') dest[j++] = strin[i];
  dest[j] = '\0';
  return (dest);
}

int input(char *s) {
  int done = 0, len = 0;
  for (int i = 0; !done && i < MAXSTR; i++) {
    if (scanf("%c", s + i) == 1 && (s[i] == '\n' || s[i] == EOF)) {
      done = 1;
      len = i;
    }
    if (i == MAXSTR - 1) {
      done = 1;
      len = MAXSTR;
    }
  }
  s[len] = '\0';
  no_space(s);
  return strlen(s);
}

int draw_txt_graph(double (*calc)(struct expr *, double),
                   struct expr *postfix) {
  double scalex = (double)(MAXX - MINX) / MAXI;
  double scaley = (double)MAXJ / (MAXY - MINY);
  int i, j;
  for (j = 0; j < MAXJ; j++) {
    double x = MINX;
    for (i = 0; i < MAXI; i++) {
      if ((int)round(calc(postfix, x) * scaley + round(MAXJ / 2)) == j) {
        printf("*");
      } else {
        printf(".");
      }
      x += scalex;
    }
    printf("\n");
  }
  return 1;
}
