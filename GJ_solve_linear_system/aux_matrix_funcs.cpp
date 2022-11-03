#include <cstdio>
#include <immintrin.h>

#include "aux_matrix_funcs.h"
#include "intrin_aux.h"
#include "non_intrin_aux.h"

void (*get_block) (double *A_src, double *dst, int i, int j, int n, int m) = nullptr;
void (*set_block) (double *A_dst, double *src, int i, int j, int n, int m) = nullptr;

void (*get_block_vec) (double *A_src, double *dst, int i, int n, int m) = nullptr;
void (*set_block_vec) (double *A_src, double *dst, int i, int n, int m) = nullptr;

void (*mult_blocks_m) (double *A, double *B, double *C, int m) = nullptr;
void (*mult_block_on_vec) (double *A, double *B, double *C, int m) = nullptr;

void (*mult_blocks_m_sub) (double *A, double *B, double *C, int m) = nullptr;
void (*mult_block_on_vec_sub) (double *A, double *B, double *C, int m) = nullptr;

void
init_aux_functions (int do_force_non_itrin)
{
  __builtin_cpu_init ();
  if (__builtin_cpu_supports ("avx") && __builtin_cpu_supports ("fma") && !do_force_non_itrin)
    {
      fprintf (stderr, "AVX&FMA are supported\n");

      mult_blocks_m = intrin_mult_blocks_m;
      mult_blocks_m_sub = intrin_mult_blocks_m_sub;
    }
  else
    {
      mult_blocks_m = non_intrin_mult_blocks_m;
      mult_blocks_m_sub = non_intrin_mult_blocks_m_sub;
    }

  get_block = non_intrin_get_block;
  set_block = non_intrin_set_block;

  get_block_vec = non_intrin_get_block_vec;
  set_block_vec = non_intrin_set_block_vec;

  mult_block_on_vec = non_intrin_mult_block_on_vec;
  mult_block_on_vec_sub = non_intrin_mult_block_on_vec_sub;
}
