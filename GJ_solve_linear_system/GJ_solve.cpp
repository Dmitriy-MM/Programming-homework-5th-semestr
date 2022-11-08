#include "GJ_solve_core.cpp"

int
solve_GJ (int n, int m, double norma, double *A, double *B_vec, double *vec_buf, double *b1, double *b2, double *b3, double *b_vec, double *b_vec2, int * seq)
{
  init_aux_functions ();
  return solve_GJ (n, m, norma, A, B_vec, vec_buf, b1, b2, b3, b_vec, b_vec2, seq, 0);
}
