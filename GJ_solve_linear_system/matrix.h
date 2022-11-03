#ifndef MATRIX
#define MATRIX
int fread_matrix (FILE *fp, double *arr, int n, int m);
void print_matrix (double *arr, int n, int m, int r = 7, FILE *fp = stdout);
double calc_residual_vec (double *vec, int n);
double calc_matrix_norm_inf (double *arr, int n, int m);
double calc_residual_vec_inf (double *vec, int n);
void print_matrix_format (double *arr, int n, int m, int r, FILE *fp = stdout);
void print_matrix_format_block (double *arr, int n, int m, int r, FILE *fp = stdout);
double calc_residual (double *A, double *B, double *b, double *C, int n, int m);
double calc_mistake (double *x, double *buf, int n);
#endif // MATRIX
