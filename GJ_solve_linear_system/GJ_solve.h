#ifndef GJ_SOLVE
#define GJ_SOLVE
#include <cstdio>
#include <memory>

// Один и тот же код одного и того же файла, просто скомпилированный по
// разному.
int accurate_solve_GJ (char *lib, int n, int m, double norma, double *A, double *B_vec, double *vec_buf, double *b1, double *b2, double *b3, double *b_vec, double *b_vec2, int * seq);

int solve_GJ (int n, int m, double norma, double *A, double *B_vec, double *vec_buf, double *b1, double *b2, double *b3, double *b_vec, double *b_vec2, int * seq);
#endif // GJ_SOLVE.
