#include <cstdio>

#include "block_storage_aux_funcs.h"
#include "matrix.h"

template <typename T>
inline static T
st_min (T a, T b)
{
  return (a > b) ? b : a;
}
template <typename T>
inline static T
st_abs (T a)
{
  return (a > 0) ? a : -a;
}

void
print_matrix_format (double *arr, int n, int m, int r, FILE *fp)
{
  int rows, cols;
  rows = st_min (n, r), cols = st_min (m, r);
  for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
        fprintf (fp, " %10.3e", arr[i * n + j]);
      fprintf (fp, "\n");
    }
}

void
print_matrix_format_block (double *arr, int n, int m, int r, FILE *fp)
{
  int rows, cols;
  rows = st_min (n, r), cols = st_min (n, r);
  for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
        fprintf (fp, " %10.3e", arr[get_pos_bs (i, j, n, m)]);
      fprintf (fp, "\n");
    }
}

double
calc_residual (double *A, double *B, double *b, double *C, int n, int m)
{
  int v = n, v4, r, q;
  double sum, s00, s10, s20, s30;
  v4 = v % 4;
  int ah = n;
  for (r = 0; r < v4; r++)
    {
      sum = -b[r];
      for (q = 0; q < ah; q++)
        sum += A[get_pos_bs (r, q, n, m)] * B[q];
      C[r] = sum;
    }
  for (; r < v; r += 4)
    {
      s00 = -b[r], s10 = -b[r + 1], s20 = -b[r + 2], s30 = -b[r + 3];
      for (q = 0; q < ah; q++)
        {
          double y = B[q];
          s00 += A[get_pos_bs (r, q, n, m)] * y;
          s10 += A[get_pos_bs (r + 1, q, n, m)] * y;
          s20 += A[get_pos_bs (r + 2, q, n, m)] * y;
          s30 += A[get_pos_bs (r + 3, q, n, m)] * y;
        }
      C[r] = s00;
      C[(r + 1)] = s10;
      C[(r + 2)] = s20;
      C[(r + 3)] = s30;
    }
  return (calc_residual_vec (C, n) / calc_residual_vec (b, n));
}

double
calc_mistake (double *x, double *buf, int n)
{
  for (int i = 0; i < n; i++)
    buf[i] -= x[i];
  return calc_residual_vec (buf, n);
}

int
fread_matrix (FILE *fp, double *arr, int n, int m)
{
  if (fp == nullptr)
    printf ("SEGFAULT SHOUD BE IN fread_matrix\n"); // Non-zero chance that
                                                    // message will be typed.
  for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < n; j++)
        if (fscanf (fp, "%lf", arr  + get_pos_bs (i, j, n, m)) != 1)
          {
            fprintf (stderr, "Can't read element i=%d, j=%d\n", i, j);
            return !0;
          }
    }
  return 0;
}

void
print_matrix (double *arr, int n, int m, int r, FILE *fp)
{
  int rows, cols;
  rows = st_min (n, r), cols = st_min (m, r);
  for (int i = 0; i < rows; i++)
    {
      for (int j = 0; j < cols; j++)
        fprintf (fp, "% -.3e ", arr[i * m + j]);
      fprintf (fp, "\n");
    }
}

// Infinity norma.
double
calc_residual_vec_inf (double *vec, int n)
{
  double res = st_abs (*vec);
  for (int i = 1; i < n; i++)
    res = (res > st_abs (vec[i])) ? res : st_abs (vec[i]);
  return res;
}

// 1 norma.
double
calc_residual_vec (double *vec, int n)
{
  double res = st_abs (*vec);
  for (int i = 1; i < n; i++)
    res += st_abs (vec[i]);
  return res;
}

double
calc_matrix_norm_inf (double *arr, int n, int m)
{
  double sum_max = -1;
  for (int i = 0; i < n; i++)
    {
      double sum = 0;
      for (int j = 0; j < m; j++)
        sum += st_abs (arr[i * m + j]);
      sum_max = sum_max < sum ? sum : sum_max;
    }
  return sum_max;
}
