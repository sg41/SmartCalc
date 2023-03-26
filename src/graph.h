#ifndef SRC_GRAPH_H_
#define SRC_GRAPH_H_
#ifndef MAXSTR
#define MAXSTR 10000
#endif
#ifdef NDEBUG
#define D(fmt, ...) \
  do {              \
  } while (0)
#else /* Not NDEBUG.  */
#define D(fmt, ...)                    \
  do {                                 \
    fprintf(stderr, fmt, __VA_ARGS__); \
  } while (0)
#endif

#define MINX -3
#define MAXX 3  // 4 * 3.14
#define MINY -3
#define MAXY 3
#define MAXI 80
#define MAXJ 25
#define MIDDLEJ 13

int input(char *s);
int draw_txt_graph();

#endif  // SRC_GRAPH_H_
