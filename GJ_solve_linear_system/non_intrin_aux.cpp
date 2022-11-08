#include <memory.h>
#include <stdio.h>

#include "block_storage_aux_funcs.h"
#include "non_intrin_aux.h"

// Получает матрицу откуда взять, буфер куда положить, (i, j) - номер блока,
// n - размерность матрицы (ожидается квадратная матрица), m - максимальная
// размерность блока.
void
non_intrin_get_block (double *A_src, double *dst, int i, int j, int n, int m)
{
  int k, l, v, h;
  int num_blocks = n / m + (n % m ? 1 : 0);
  double *pA = A_src + get_shift_via_block_coords (i, j, num_blocks, n, m);
  k = n / m;
  l = n % m;                                // Столько не влезло в последний блок по одной стороне.
  v = (i < k ? m : l), h = (j < k ? m : l); // Размеры блока.
  for (int t = 0; t < m * m; t++)
    dst[t] = 0.; // Размер буфера ВСЕГДА m*m

  for (int s = 0; s < v; s++)
    memcpy (dst + s * m, pA + s * h, sizeof (double) * h);
}

// A_src, dst - вектора размерностей n и m соответственно.
void
non_intrin_get_block_vec (double *A_src, double *dst, int i, int n, int m)
{
  double *pA = A_src + i * m;
  int m_real = (n - i * m) < m ? n - i * m : m;
  for (int t = 0; t < m; t++)
    dst[t] = 0.;
  memcpy (dst, pA, sizeof (double) * m_real);
}

void
non_intrin_set_block (double *A_dst, double *src, int i, int j, int n, int m)
{
  int k, l, v, h;
  int num_blocks = n / m + (n % m ? 1 : 0);
  double *pA = A_dst + get_shift_via_block_coords (i, j, num_blocks, n, m);

  k = n / m;
  l = n % m;                                // Столько не влезло в последний блок по одной стороне.
  v = (i < k ? m : l), h = (j < k ? m : l); // Размеры блока.

  for (int t = 0; t < v; t++)
    memcpy (pA + t * h, src + t * m, sizeof (double) * h);
}

void
non_intrin_set_block_vec (double *A_src, double *dst, int i, int n, int m)
{
  dst += i * m;
  double m_real = (n - i * m < m) ? n - i * m : m;
  memcpy (dst, A_src, sizeof (double) * m_real);
  
}

// Блок на вектор.
void
non_intrin_mult_block_on_vec (double *A, double *B, double *C, int m)
{
  int v = m, v4, r, q;
  double sum, s00, s10, s20, s30, s40, s50;
  v4 = v % 4;
  (void)s50;
  (void)s40;
  int ah = m;
  for (r = 0; r < v4; r++)
    {
      sum = 0;
      for (q = 0; q < ah; q++)
        sum += A[r * m + q] * B[q];
      C[r] = sum;
    }
  for (; r < v; r += 4)
    {
      s00 = (s10 = (s20 = (s30 = (s40 = (s50 = 0)))));
      for (q = 0; q < ah; q++)
        {
          double y = B[q];
          s00 += A[r * m + q] * y;       // c_r_t
          s10 += A[(r + 1) * m + q] * y; // c_r+1_t
          s20 += A[(r + 2) * m + q] * y; // c_r+2_t
          s30 += A[(r + 3) * m + q] * y;
          // s40 += A[(r+4)*m + q] * y;
          // s50 += A[(r+5)*m + q] * y;
        }
      C[r] = s00;
      C[(r + 1)] = s10;
      C[(r + 2)] = s20;
      C[(r + 3)] = s30;
      // C[(r+4)] = s20;
      // C[(r+5)] = s20;
    }
}

// Перемножение двух блоков размерности m.
void
non_intrin_mult_blocks_m (double *A, double *B, double *C, int m)
{
  int v = m, h = m, v3, h3, t, r, q;
  double sum, s00, s01, s02, s10, s11, s12, s20, s21, s22;
  v3 = v % 3;
  h3 = h % 3;
  int ah = m;
  for (r = 0; r < v3; r++)
    {
      for (t = 0; t < h3; t++) // Диагональный уголок где не поделилась ни строки, ни столбцы.
        {
          sum = 0;
          for (q = 0; q < ah; q++)
            sum += A[r * m + q] * B[q * m + t];
          C[r * m + t] = sum;
        }
      for (; t < h; t += 3) // Проход по первым строчкам (не поделившимся) по
                            // всем оставшимся столбцам.
        {
          s00 = (s01 = (s02 = 0));
          for (q = 0; q < ah; q++)
            {
              double x = A[r * m + q];
              s00 += x * B[q * m + t];     // c_r_t
              s01 += x * B[q * m + t + 1]; // c_r_t+1
              s02 += x * B[q * m + t + 2]; // c_r_t+1
            }
          C[r * m + t] = s00, C[r * m + t + 1] = s01, C[r * m + t + 2] = s02;
        }
    }
  for (; r < v; r += 3)
    {
      for (t = 0; t < h3; t++) // Строки поделились, но не поделились столбцы,
                               // т.е. это маленький левый столбик матрицы, без
                               // включения первых элементов.
        {
          s00 = (s10 = (s20 = 0));
          for (q = 0; q < ah; q++)
            {
              double y = B[q * m + t];
              s00 += A[r * m + q] * y;       // c_r_t
              s10 += A[(r + 1) * m + q] * y; // c_r+1_t
              s20 += A[(r + 2) * m + q] * y; // c_r+2_t
            }
          C[r * m + t] = s00;
          C[(r + 1) * m + t] = s10;
          C[(r + 2) * m + t] = s20;
        }
      for (; t < h; t += 3) // Проход по блокам 3x3.
        {
          s00 = (s01 = (s02 = (s10 = (s11 = (s12 = (s20 = (s21 = (s22 = 0))))))));
          for (q = 0; q < ah; q++)
            {
              double x1 = A[r * m + q], x2 = A[(r + 1) * m + q],
                     x3 = A[(r + 2) * m + q]; // Замедлило программу!
              double y1 = B[q * m + t], y2 = B[q * m + t + 1], y3 = B[q * m + t + 2];

              s00 += x1 * y1; // c_r_t
              s01 += x1 * y2; // c_r_t+1
              s02 += x1 * y3; // c_r_t+1

              s10 += x2 * y1; // c_r+1_t
              s11 += x2 * y2; // c_r+1_t+1
              s12 += x2 * y3; // c_r+1_t+1

              s20 += x3 * y1; // c_r+2_t
              s21 += x3 * y2; // c_r+2_t+1
              s22 += x3 * y3; // c_r+2_t+1
            }
          C[r * m + t] = s00, C[r * m + t + 1] = s01, C[r * m + t + 2] = s02, C[(r + 1) * m + t] = s10, C[(r + 1) * m + t + 1] = s11, C[(r + 1) * m + t + 2] = s12, C[(r + 2) * m + t] = s20,
                    C[(r + 2) * m + t + 1] = s21, C[(r + 2) * m + t + 2] = s22;
        }
    }
}

void
non_intrin_mult_blocks_m_sub (double *A, double *B, double *C, int m)
{
  int v = m, h = m, v3, h3, t, r, q;
  double sum, s00, s01, s02, s10, s11, s12, s20, s21, s22;
  v3 = v % 3;
  h3 = h % 3;
  int ah = m;
  for (r = 0; r < v3; r++)
    {
      for (t = 0; t < h3; t++) // Диагональный уголок где не поделилась ни строки, ни столбцы.
        {
          sum = 0;
          for (q = 0; q < ah; q++)
            sum += A[r * m + q] * B[q * m + t];
          C[r * m + t] -= sum;
        }
      for (; t < h; t += 3) // Проход по первым строчкам (не поделившимся) по
                            // всем оставшимся столбцам.
        {
          s00 = (s01 = (s02 = 0));
          for (q = 0; q < ah; q++)
            {
              double x = A[r * m + q];
              s00 += x * B[q * m + t];     // c_r_t
              s01 += x * B[q * m + t + 1]; // c_r_t+1
              s02 += x * B[q * m + t + 2]; // c_r_t+1
            }
          C[r * m + t] -= s00, C[r * m + t + 1] -= s01, C[r * m + t + 2] -= s02;
        }
    }
  for (; r < v; r += 3)
    {
      for (t = 0; t < h3; t++) // Строки поделились, но не поделились столбцы,
                               // т.е. это маленький левый столбик матрицы, без
                               // включения первых элементов.
        {
          s00 = (s10 = (s20 = 0));
          for (q = 0; q < ah; q++)
            {
              double y = B[q * m + t];
              s00 += A[r * m + q] * y;       // c_r_t
              s10 += A[(r + 1) * m + q] * y; // c_r+1_t
              s20 += A[(r + 2) * m + q] * y; // c_r+2_t
            }
          C[r * m + t] -= s00;
          C[(r + 1) * m + t] -= s10;
          C[(r + 2) * m + t] -= s20;
        }
      for (; t < h; t += 3) // Проход по блокам 3x3.
        {
          s00 = (s01 = (s02 = (s10 = (s11 = (s12 = (s20 = (s21 = (s22 = 0))))))));
          for (q = 0; q < ah; q++)
            {
              double x1 = A[r * m + q], x2 = A[(r + 1) * m + q],
                     x3 = A[(r + 2) * m + q]; // Замедлило программу!
              double y1 = B[q * m + t], y2 = B[q * m + t + 1], y3 = B[q * m + t + 2];

              s00 += x1 * y1; // c_r_t
              s01 += x1 * y2; // c_r_t+1
              s02 += x1 * y3; // c_r_t+1

              s10 += x2 * y1; // c_r+1_t
              s11 += x2 * y2; // c_r+1_t+1
              s12 += x2 * y3; // c_r+1_t+1

              s20 += x3 * y1; // c_r+2_t
              s21 += x3 * y2; // c_r+2_t+1
              s22 += x3 * y3; // c_r+2_t+1
            }
          C[r * m + t] -= s00, C[r * m + t + 1] -= s01, C[r * m + t + 2] -= s02, C[(r + 1) * m + t] -= s10, C[(r + 1) * m + t + 1] -= s11, C[(r + 1) * m + t + 2] -= s12, C[(r + 2) * m + t] -= s20,
              C[(r + 2) * m + t + 1] -= s21, C[(r + 2) * m + t + 2] -= s22;
        }
    }
}

void
non_intrin_mult_block_on_vec_sub (double *A, double *B, double *C, int m)
{
  int v = m, v4, r, q;
  double sum, s00, s10, s20, s30, s40, s50;
  v4 = v % 4;
  (void)s50;
  (void)s40;
  int ah = m;
  for (r = 0; r < v4; r++)
    {
      sum = 0;
      for (q = 0; q < ah; q++)
        sum += A[r * m + q] * B[q];
      C[r] -= sum;
    }
  for (; r < v; r += 4)
    {
      s00 = (s10 = (s20 = (s30 = (s40 = (s50 = 0)))));
      for (q = 0; q < ah; q++)
        {
          double y = B[q];
          s00 += A[r * m + q] * y;       // c_r_t
          s10 += A[(r + 1) * m + q] * y; // c_r+1_t
          s20 += A[(r + 2) * m + q] * y; // c_r+2_t
          s30 += A[(r + 3) * m + q] * y;
          // s40 += A[(r+4)*m + q] * y;
          // s50 += A[(r+5)*m + q] * y;
        }
      C[r] -= s00;
      C[(r + 1)] -= s10;
      C[(r + 2)] -= s20;
      C[(r + 3)] -= s30;
      // C[(r+4)] = s20;
      // C[(r+5)] = s20;
    }
}
