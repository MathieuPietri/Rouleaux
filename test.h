#ifndef GLPK
#define GLPK
#include <glpk.h>		/* GNU GLPK linear/mixed integer solver */
#endif

int *column_generator();
int add_column(glp_prob *lp, float coef);
int add_column2(glp_prob *lp, float coef);