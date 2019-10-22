#ifndef GLPK
#define GLPK
#include <glpk.h>		/* GNU GLPK linear/mixed integer solver */
#endif

void column_generator();
void add_column(glp_prob *lp, float coef);