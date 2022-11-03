#include <immintrin.h>

#include "intrin_aux.h"

inline static void
micro_6x16 (int m, double *A, int step, double *B, double *C)
{
  __m256d c00 = _mm256_setzero_pd ();
  __m256d c01 = _mm256_setzero_pd ();
  __m256d c02 = _mm256_setzero_pd ();
  __m256d c03 = _mm256_setzero_pd ();
  __m256d c10 = _mm256_setzero_pd ();
  __m256d c11 = _mm256_setzero_pd ();
  __m256d c12 = _mm256_setzero_pd ();
  __m256d c13 = _mm256_setzero_pd ();
  __m256d c20 = _mm256_setzero_pd ();
  __m256d c21 = _mm256_setzero_pd ();
  __m256d c22 = _mm256_setzero_pd ();
  __m256d c23 = _mm256_setzero_pd ();
  __m256d c30 = _mm256_setzero_pd ();
  __m256d c31 = _mm256_setzero_pd ();
  __m256d c32 = _mm256_setzero_pd ();
  __m256d c33 = _mm256_setzero_pd ();
  __m256d c40 = _mm256_setzero_pd ();
  __m256d c41 = _mm256_setzero_pd ();
  __m256d c42 = _mm256_setzero_pd ();
  __m256d c43 = _mm256_setzero_pd ();
  __m256d c50 = _mm256_setzero_pd ();
  __m256d c51 = _mm256_setzero_pd ();
  __m256d c52 = _mm256_setzero_pd ();
  __m256d c53 = _mm256_setzero_pd ();
  int offset0 = m * 0;
  int offset1 = m * 1;
  int offset2 = m * 2;
  int offset3 = m * 3;
  int offset4 = m * 4;
  int offset5 = m * 5;
  __m256d b0, a0;
  __m256d b1, a1;
  __m256d b2;
  __m256d b3;
  for (int k = 0; k < m; k++)
    {
      b0 = _mm256_loadu_pd (B + 0);
      b1 = _mm256_loadu_pd (B + 4);
      b2 = _mm256_loadu_pd (B + 8);
      b3 = _mm256_loadu_pd (B + 12);
      a0 = _mm256_set1_pd (A[offset0]);
      a1 = _mm256_set1_pd (A[offset1]);
      c00 = _mm256_fmadd_pd (a0, b0, c00);
      c01 = _mm256_fmadd_pd (a0, b1, c01);
      c02 = _mm256_fmadd_pd (a0, b2, c02);
      c03 = _mm256_fmadd_pd (a0, b3, c03);
      c10 = _mm256_fmadd_pd (a1, b0, c10);
      c11 = _mm256_fmadd_pd (a1, b1, c11);
      c12 = _mm256_fmadd_pd (a1, b2, c12);
      c13 = _mm256_fmadd_pd (a1, b3, c13);
      a0 = _mm256_set1_pd (A[offset2]);
      a1 = _mm256_set1_pd (A[offset3]);
      c20 = _mm256_fmadd_pd (a0, b0, c20);
      c21 = _mm256_fmadd_pd (a0, b1, c21);
      c22 = _mm256_fmadd_pd (a0, b2, c22);
      c23 = _mm256_fmadd_pd (a0, b3, c23);
      c30 = _mm256_fmadd_pd (a1, b0, c30);
      c31 = _mm256_fmadd_pd (a1, b1, c31);
      c32 = _mm256_fmadd_pd (a1, b2, c32);
      c33 = _mm256_fmadd_pd (a1, b3, c33);
      a0 = _mm256_set1_pd (A[offset4]);
      a1 = _mm256_set1_pd (A[offset5]);
      c40 = _mm256_fmadd_pd (a0, b0, c40);
      c41 = _mm256_fmadd_pd (a0, b1, c41);
      c42 = _mm256_fmadd_pd (a0, b2, c42);
      c43 = _mm256_fmadd_pd (a0, b3, c43);
      c50 = _mm256_fmadd_pd (a1, b0, c50);
      c51 = _mm256_fmadd_pd (a1, b1, c51);
      c52 = _mm256_fmadd_pd (a1, b2, c52);
      c53 = _mm256_fmadd_pd (a1, b3, c53);
      B += m;
      A += step;
    }
  _mm256_storeu_pd (C + 0, _mm256_add_pd (c00, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_add_pd (c01, _mm256_loadu_pd (C + 4)));
  _mm256_storeu_pd (C + 8, _mm256_add_pd (c02, _mm256_loadu_pd (C + 8)));
  _mm256_storeu_pd (C + 12, _mm256_add_pd (c03, _mm256_loadu_pd (C + 12)));
  C += m;
  _mm256_storeu_pd (C + 0, _mm256_add_pd (c10, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_add_pd (c11, _mm256_loadu_pd (C + 4)));
  _mm256_storeu_pd (C + 8, _mm256_add_pd (c12, _mm256_loadu_pd (C + 8)));
  _mm256_storeu_pd (C + 12, _mm256_add_pd (c13, _mm256_loadu_pd (C + 12)));
  C += m;
  _mm256_storeu_pd (C + 0, _mm256_add_pd (c20, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_add_pd (c21, _mm256_loadu_pd (C + 4)));
  _mm256_storeu_pd (C + 8, _mm256_add_pd (c22, _mm256_loadu_pd (C + 8)));
  _mm256_storeu_pd (C + 12, _mm256_add_pd (c23, _mm256_loadu_pd (C + 12)));
  C += m;
  _mm256_storeu_pd (C + 0, _mm256_add_pd (c30, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_add_pd (c31, _mm256_loadu_pd (C + 4)));
  _mm256_storeu_pd (C + 8, _mm256_add_pd (c32, _mm256_loadu_pd (C + 8)));
  _mm256_storeu_pd (C + 12, _mm256_add_pd (c33, _mm256_loadu_pd (C + 12)));
  C += m;
  _mm256_storeu_pd (C + 0, _mm256_add_pd (c40, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_add_pd (c41, _mm256_loadu_pd (C + 4)));
  _mm256_storeu_pd (C + 8, _mm256_add_pd (c42, _mm256_loadu_pd (C + 8)));
  _mm256_storeu_pd (C + 12, _mm256_add_pd (c43, _mm256_loadu_pd (C + 12)));
  C += m;
  _mm256_storeu_pd (C + 0, _mm256_add_pd (c50, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_add_pd (c51, _mm256_loadu_pd (C + 4)));
  _mm256_storeu_pd (C + 8, _mm256_add_pd (c52, _mm256_loadu_pd (C + 8)));
  _mm256_storeu_pd (C + 12, _mm256_add_pd (c53, _mm256_loadu_pd (C + 12)));
}

inline static void
init_c (int M, int N, double *C, int m)
{
  // M - высота блока (4)
  // N - длина блока (8)
  // C - адрес блока в выходной матрице
  // m - длина строчки выходной матрицы (т.е. m)
  for (int i = 0; i < M; i++, C += m)
    for (int j = 0; j < N; j += 4)
      _mm256_storeu_pd (C + j, _mm256_setzero_pd ());
}

inline static void
micro_4x8 (int m, double *A, int step, double *B, double *C)
{
  __m256d c00 = _mm256_setzero_pd ();
  __m256d c01 = _mm256_setzero_pd ();
  __m256d c10 = _mm256_setzero_pd ();
  __m256d c11 = _mm256_setzero_pd ();
  __m256d c20 = _mm256_setzero_pd ();
  __m256d c21 = _mm256_setzero_pd ();
  __m256d c30 = _mm256_setzero_pd ();
  __m256d c31 = _mm256_setzero_pd ();
  int offset0 = m * 0;
  int offset1 = m * 1;
  int offset2 = m * 2;
  int offset3 = m * 3;
  __m256d b0, a0;
  __m256d b1, a1;
  for (int k = 0; k < m; k++)
    {
      b0 = _mm256_loadu_pd (B + 0);
      b1 = _mm256_loadu_pd (B + 4);
      a0 = _mm256_set1_pd (A[offset0]);
      a1 = _mm256_set1_pd (A[offset1]);
      c00 = _mm256_fmadd_pd (a0, b0, c00);
      c01 = _mm256_fmadd_pd (a0, b1, c01);
      c10 = _mm256_fmadd_pd (a1, b0, c10);
      c11 = _mm256_fmadd_pd (a1, b1, c11);
      a0 = _mm256_set1_pd (A[offset2]);
      a1 = _mm256_set1_pd (A[offset3]);
      c20 = _mm256_fmadd_pd (a0, b0, c20);
      c21 = _mm256_fmadd_pd (a0, b1, c21);
      c30 = _mm256_fmadd_pd (a1, b0, c30);
      c31 = _mm256_fmadd_pd (a1, b1, c31);
      B += m;
      A += step;
    }
  _mm256_storeu_pd (C + 0, _mm256_add_pd (c00, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_add_pd (c01, _mm256_loadu_pd (C + 4)));
  C += m;
  _mm256_storeu_pd (C + 0, _mm256_add_pd (c10, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_add_pd (c11, _mm256_loadu_pd (C + 4)));
  C += m;
  _mm256_storeu_pd (C + 0, _mm256_add_pd (c20, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_add_pd (c21, _mm256_loadu_pd (C + 4)));
  C += m;
  _mm256_storeu_pd (C + 0, _mm256_add_pd (c30, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_add_pd (c31, _mm256_loadu_pd (C + 4)));
}

void
intrin_mult_blocks_m (double *A, double *B, double *C, int m)
{
  int m8 = m / 8;
  int m4 = m / 4;

  for (int i = 0; i < m4; i++)
    for (int j = 0; j < m8; j++)
      {
        init_c (4, 8, C + i * 4 * m + 8 * j, m);
        micro_4x8 (m, A + i * 4 * m, 1, B + 8 * j, C + i * 4 * m + 8 * j);
      }
}

inline static void
micro_6x16_sub (int m, double *A, int step, double *B, double *C)
{
  __m256d c00 = _mm256_setzero_pd ();
  __m256d c01 = _mm256_setzero_pd ();
  __m256d c02 = _mm256_setzero_pd ();
  __m256d c03 = _mm256_setzero_pd ();
  __m256d c10 = _mm256_setzero_pd ();
  __m256d c11 = _mm256_setzero_pd ();
  __m256d c12 = _mm256_setzero_pd ();
  __m256d c13 = _mm256_setzero_pd ();
  __m256d c20 = _mm256_setzero_pd ();
  __m256d c21 = _mm256_setzero_pd ();
  __m256d c22 = _mm256_setzero_pd ();
  __m256d c23 = _mm256_setzero_pd ();
  __m256d c30 = _mm256_setzero_pd ();
  __m256d c31 = _mm256_setzero_pd ();
  __m256d c32 = _mm256_setzero_pd ();
  __m256d c33 = _mm256_setzero_pd ();
  __m256d c40 = _mm256_setzero_pd ();
  __m256d c41 = _mm256_setzero_pd ();
  __m256d c42 = _mm256_setzero_pd ();
  __m256d c43 = _mm256_setzero_pd ();
  __m256d c50 = _mm256_setzero_pd ();
  __m256d c51 = _mm256_setzero_pd ();
  __m256d c52 = _mm256_setzero_pd ();
  __m256d c53 = _mm256_setzero_pd ();
  int offset0 = m * 0;
  int offset1 = m * 1;
  int offset2 = m * 2;
  int offset3 = m * 3;
  int offset4 = m * 4;
  int offset5 = m * 5;
  __m256d b0, a0;
  __m256d b1, a1;
  __m256d b2;
  __m256d b3;
  for (int k = 0; k < m; k++)
    {
      b0 = _mm256_loadu_pd (B + 0);
      b1 = _mm256_loadu_pd (B + 4);
      b2 = _mm256_loadu_pd (B + 8);
      b3 = _mm256_loadu_pd (B + 12);
      a0 = _mm256_set1_pd (A[offset0]);
      a1 = _mm256_set1_pd (A[offset1]);
      c00 = _mm256_fmadd_pd (a0, b0, c00);
      c01 = _mm256_fmadd_pd (a0, b1, c01);
      c02 = _mm256_fmadd_pd (a0, b2, c02);
      c03 = _mm256_fmadd_pd (a0, b3, c03);
      c10 = _mm256_fmadd_pd (a1, b0, c10);
      c11 = _mm256_fmadd_pd (a1, b1, c11);
      c12 = _mm256_fmadd_pd (a1, b2, c12);
      c13 = _mm256_fmadd_pd (a1, b3, c13);
      a0 = _mm256_set1_pd (A[offset2]);
      a1 = _mm256_set1_pd (A[offset3]);
      c20 = _mm256_fmadd_pd (a0, b0, c20);
      c21 = _mm256_fmadd_pd (a0, b1, c21);
      c22 = _mm256_fmadd_pd (a0, b2, c22);
      c23 = _mm256_fmadd_pd (a0, b3, c23);
      c30 = _mm256_fmadd_pd (a1, b0, c30);
      c31 = _mm256_fmadd_pd (a1, b1, c31);
      c32 = _mm256_fmadd_pd (a1, b2, c32);
      c33 = _mm256_fmadd_pd (a1, b3, c33);
      a0 = _mm256_set1_pd (A[offset4]);
      a1 = _mm256_set1_pd (A[offset5]);
      c40 = _mm256_fmadd_pd (a0, b0, c40);
      c41 = _mm256_fmadd_pd (a0, b1, c41);
      c42 = _mm256_fmadd_pd (a0, b2, c42);
      c43 = _mm256_fmadd_pd (a0, b3, c43);
      c50 = _mm256_fmadd_pd (a1, b0, c50);
      c51 = _mm256_fmadd_pd (a1, b1, c51);
      c52 = _mm256_fmadd_pd (a1, b2, c52);
      c53 = _mm256_fmadd_pd (a1, b3, c53);
      B += m;
      A += step;
    }
  _mm256_storeu_pd (C + 0, _mm256_sub_pd (c00, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_sub_pd (c01, _mm256_loadu_pd (C + 4)));
  _mm256_storeu_pd (C + 8, _mm256_sub_pd (c02, _mm256_loadu_pd (C + 8)));
  _mm256_storeu_pd (C + 12, _mm256_sub_pd (c03, _mm256_loadu_pd (C + 12)));
  C += m;
  _mm256_storeu_pd (C + 0, _mm256_sub_pd (c10, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_sub_pd (c11, _mm256_loadu_pd (C + 4)));
  _mm256_storeu_pd (C + 8, _mm256_sub_pd (c12, _mm256_loadu_pd (C + 8)));
  _mm256_storeu_pd (C + 12, _mm256_sub_pd (c13, _mm256_loadu_pd (C + 12)));
  C += m;
  _mm256_storeu_pd (C + 0, _mm256_add_pd (c20, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_add_pd (c21, _mm256_loadu_pd (C + 4)));
  _mm256_storeu_pd (C + 8, _mm256_add_pd (c22, _mm256_loadu_pd (C + 8)));
  _mm256_storeu_pd (C + 12, _mm256_add_pd (c23, _mm256_loadu_pd (C + 12)));
  C += m;
  _mm256_storeu_pd (C + 0, _mm256_sub_pd (c30, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_sub_pd (c31, _mm256_loadu_pd (C + 4)));
  _mm256_storeu_pd (C + 8, _mm256_sub_pd (c32, _mm256_loadu_pd (C + 8)));
  _mm256_storeu_pd (C + 12, _mm256_sub_pd (c33, _mm256_loadu_pd (C + 12)));
  C += m;
  _mm256_storeu_pd (C + 0, _mm256_sub_pd (c40, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_sub_pd (c41, _mm256_loadu_pd (C + 4)));
  _mm256_storeu_pd (C + 8, _mm256_sub_pd (c42, _mm256_loadu_pd (C + 8)));
  _mm256_storeu_pd (C + 12, _mm256_sub_pd (c43, _mm256_loadu_pd (C + 12)));
  C += m;
  _mm256_storeu_pd (C + 0, _mm256_sub_pd (c50, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_sub_pd (c51, _mm256_loadu_pd (C + 4)));
  _mm256_storeu_pd (C + 8, _mm256_sub_pd (c52, _mm256_loadu_pd (C + 8)));
  _mm256_storeu_pd (C + 12, _mm256_sub_pd (c53, _mm256_loadu_pd (C + 12)));
}

inline static void
micro_4x8_sub (int m, double *A, int step, double *B, double *C)
{
  __m256d c00 = _mm256_setzero_pd ();
  __m256d c01 = _mm256_setzero_pd ();
  __m256d c10 = _mm256_setzero_pd ();
  __m256d c11 = _mm256_setzero_pd ();
  __m256d c20 = _mm256_setzero_pd ();
  __m256d c21 = _mm256_setzero_pd ();
  __m256d c30 = _mm256_setzero_pd ();
  __m256d c31 = _mm256_setzero_pd ();
  int offset0 = m * 0;
  int offset1 = m * 1;
  int offset2 = m * 2;
  int offset3 = m * 3;
  __m256d b0, a0;
  __m256d b1, a1;
  for (int k = 0; k < m; k++)
    {
      b0 = _mm256_loadu_pd (B + 0);
      b1 = _mm256_loadu_pd (B + 4);
      a0 = _mm256_set1_pd (-A[offset0]);
      a1 = _mm256_set1_pd (-A[offset1]);
      c00 = _mm256_fmadd_pd (a0, b0, c00);
      c01 = _mm256_fmadd_pd (a0, b1, c01);
      c10 = _mm256_fmadd_pd (a1, b0, c10);
      c11 = _mm256_fmadd_pd (a1, b1, c11);
      a0 = _mm256_set1_pd (-A[offset2]);
      a1 = _mm256_set1_pd (-A[offset3]);
      c20 = _mm256_fmadd_pd (a0, b0, c20);
      c21 = _mm256_fmadd_pd (a0, b1, c21);
      c30 = _mm256_fmadd_pd (a1, b0, c30);
      c31 = _mm256_fmadd_pd (a1, b1, c31);
      B += m;
      A += step;
    }
  _mm256_storeu_pd (C + 0, _mm256_add_pd (c00, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_add_pd (c01, _mm256_loadu_pd (C + 4)));
  C += m;
  _mm256_storeu_pd (C + 0, _mm256_add_pd (c10, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_add_pd (c11, _mm256_loadu_pd (C + 4)));
  C += m;
  _mm256_storeu_pd (C + 0, _mm256_add_pd (c20, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_add_pd (c21, _mm256_loadu_pd (C + 4)));
  C += m;
  _mm256_storeu_pd (C + 0, _mm256_add_pd (c30, _mm256_loadu_pd (C + 0)));
  _mm256_storeu_pd (C + 4, _mm256_add_pd (c31, _mm256_loadu_pd (C + 4)));
}

void
intrin_mult_blocks_m_sub (double *A, double *B, double *C, int m)
{
  int m8 = m / 8;
  int m4 = m / 4;

  for (int i = 0; i < m4; i++)
    for (int j = 0; j < m8; j++)
      micro_4x8_sub (m, A + i * 4 * m, 1, B + 8 * j, C + i * 4 * m + 8 * j);
}
