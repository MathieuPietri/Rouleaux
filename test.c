/* short.c */

#include <stdio.h>		/* C input/output                       */
#include <stdlib.h>		/* C standard library                   */
#include <glpk.h>		/* GNU GLPK linear/mixed integer solver */
#include "test.h"
int col_num = 1;
int num_col_sp = 1;
int *column_generator(int y1, int y2, int y3, int y4) {

	glp_prob *sous_prob;
	int ia[1 + 1000], ja[1 + 1000];
	double ar[1 + 1000], z, x1, x2, x3, x4;
	/* create problem */
	sous_prob = glp_create_prob();
	glp_set_prob_name(sous_prob, "rouleaux-parfaits");
	glp_set_obj_dir(sous_prob, GLP_MAX);
	/* fill problem */
	glp_add_rows(sous_prob, 4);
	glp_set_row_name(sous_prob, 1, "taille0");
	glp_set_row_bnds(sous_prob, 1, GLP_UP,  0.0, 100.0);

	num_col_sp = 1;
	add_column_sp(sous_prob);
	add_column_sp(sous_prob);
	add_column_sp(sous_prob);
	add_column_sp(sous_prob);
	glp_set_col_kind(sous_prob, 1, GLP_IV);
	glp_set_col_kind(sous_prob, 2, GLP_IV);
	glp_set_col_kind(sous_prob, 3, GLP_IV);
	glp_set_col_kind(sous_prob, 4, GLP_IV);

	ia[1] = 1, ja[1] = 1, ar[1] = 45.0;	/* a[1,1] = a1 * 45 */
	ia[2] = 1, ja[2] = 2, ar[2] = 36.0;	/* a[1,2] = a2 * 36 */
	ia[3] = 1, ja[3] = 3, ar[3] = 31.0;	/* a[1,3] = a3 * 31 */
	ia[4] = 1, ja[4] = 4, ar[4] = 14.0;	/* a[1,4] = a4 * 14 */
	glp_load_matrix(sous_prob, 4, ia, ja, ar);
	
	/* solve problem */

	glp_simplex(sous_prob, NULL);
	glp_intopt(sous_prob, NULL);

	int *a = malloc(sizeof (int) * 5);

	a[0] = glp_get_obj_val(sous_prob);
	a[1] = glp_mip_col_val(sous_prob, 1);
	a[2] = glp_mip_col_val(sous_prob, 2);
	a[3] = glp_mip_col_val(sous_prob, 3);
	a[4] = glp_mip_col_val(sous_prob, 4);

	printf("a1 = %d; a2 = %d; a3 = %d; a4 = %d\n", a[1], a[2], a[3], a[4]);
	/* housekeeping */
	glp_delete_prob(sous_prob);
	glp_free_env();
	
	return a;
	//glp_intopt(glp_prob *mip, const glp_iocp *parm);

}

int perfect_rolls() {
	/* declare variables */
	glp_prob *lp;
	int ia[1 + 1000], ja[1 + 1000];
	double ar[1 + 1000], z, x1, x2, x3, x4;
	/* create problem */
	lp = glp_create_prob();
	glp_set_prob_name(lp, "rouleaux-parfaits");
	glp_set_obj_dir(lp, GLP_MIN);
	/* fill problem */
	glp_add_rows(lp, 4);
	glp_set_row_name(lp, 1, "taille0");
	glp_set_row_bnds(lp, 1, GLP_LO,  97.0, 0.0);
	glp_set_row_name(lp, 2, "taille1");
	glp_set_row_bnds(lp, 2, GLP_LO, 610.0, 0.0);
	glp_set_row_name(lp, 3, "taille2");
	glp_set_row_bnds(lp, 3, GLP_LO, 395.0, 0.0);
	glp_set_row_name(lp, 4, "taille3");
	glp_set_row_bnds(lp, 4, GLP_LO, 211.0, 0.0);

	add_column(lp, 1.0);
	add_column(lp, 1.0);
	add_column(lp, 1.0);
	add_column(lp, 1.0);

	ia[1] = 1, ja[1] = 1, ar[1] = 1.0;	/* a[1,1] = 1 */
	ia[2] = 2, ja[2] = 2, ar[2] = 1.0;	/* a[2,2] = 1 */
	ia[3] = 3, ja[3] = 3, ar[3] = 1.0;	/* a[3,3] = 1 */
	ia[4] = 4, ja[4] = 4, ar[4] = 1.0;	/* a[4,4] = 1 */


	
	int nb_coeffs = 4;
	double y1, y2, y3, y4;
	int *a, z_cost;
	do {
		printf("nb_coeffs=%d\n", nb_coeffs);
		for (int i = 0; i < nb_coeffs + 4; i++) {
			printf("ar[%d]=%g\n", i, ar[i]);
		}
		printf("\n");

		glp_load_matrix(lp, nb_coeffs, ia, ja, ar);
		printf("col_num=%d\n", col_num);
		/* solve problem */
		glp_simplex(lp, NULL);
		y1 = glp_get_row_dual(lp, 1);
		y2 = glp_get_row_dual(lp, 2);
		y3 = glp_get_row_dual(lp, 3);
		y4 = glp_get_row_dual(lp, 4);
		printf("z = %g; y1 = %g; y2 = %g; y3 = %g; y4 = %g\n", z, y1, y2, y3,
			y4);
		a = column_generator(y1, y2, y3, y4);
		z_cost = a[0];
		if (z_cost > 1) {
			for (int i = 1; i <= 4; i++)
				printf("a[%d]=%d\n", i, a[i]);
			add_column(lp, 1.0);
			ia[++nb_coeffs] = 1, ja[nb_coeffs] = col_num-1, ar[nb_coeffs] = (double) a[1];	/* a[col_num,1] = a[1] */
			ia[++nb_coeffs] = 2, ja[nb_coeffs] = col_num-1, ar[nb_coeffs] = (double) a[2];	/* a[col_num,2] = a[2] */
			ia[++nb_coeffs] = 3, ja[nb_coeffs] = col_num-1, ar[nb_coeffs] = (double) a[3];	/* a[col_num,3] = a[3] */
			ia[++nb_coeffs] = 4, ja[nb_coeffs] = col_num-1, ar[nb_coeffs] = (double) a[4];	/* a[col_num,4] = a[4] */
		}
		free(a);
	} while (z_cost > 1);


	/* recover and display results */
	z = glp_get_obj_val(lp);
	x1 = glp_get_col_prim(lp, 1);
	x2 = glp_get_col_prim(lp, 2);
	x3 = glp_get_col_prim(lp, 3);
	x4 = glp_get_col_prim(lp, 4);
	printf("z = %g; x1 = %g; x2 = %g; x3 = %g; x4 = %g\n", z, x1, x2, x3,
		   x4);


	/* housekeeping */
	glp_delete_prob(lp);
	glp_free_env();

	return z;
}


void add_column(glp_prob *lp, float coef) {
	glp_add_cols(lp, 1);
	char name[10] = "";
	snprintf(name, sizeof name, "rouleau%d", col_num);
	glp_set_col_name(lp, col_num, name);
	glp_set_col_bnds(lp, col_num, GLP_LO, 0.0, 0.0);
	glp_set_obj_coef(lp, col_num, coef);

	col_num++;
}

void add_column_sp(glp_prob *lp) {
	glp_add_cols(lp, 1);
	char name[10] = "";
	snprintf(name, sizeof name, "rouleau%d", num_col_sp);
	glp_set_col_name(lp, num_col_sp, name);
	glp_set_col_bnds(lp, num_col_sp, GLP_LO, 0.0, 0.0);
	glp_set_obj_coef(lp, num_col_sp, 1.0);

	num_col_sp++;
}

int main(void) {
	perfect_rolls();
	return 0;
}
