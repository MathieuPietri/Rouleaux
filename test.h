#ifndef GLPK
#define GLPK
#include <glpk.h>		/* GNU GLPK linear/mixed integer solver */
#endif

int *column_generator();
void add_column(glp_prob *lp, float coef);
void add_column_sp(glp_prob *lp);