#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <memory.h>
#include <mm_malloc.h>

#include "GJ_solve.h"
#include "block_storage_aux_funcs.h"
#include "matrix.h"

#define MICRO_DIMENSION (8)
#define INTRIN_SIZE (256)

template <typename T>
static T
st_max (T a, T b)
{
  return (a > b) ? a : b;
}
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

static int
st_init_matrix (double *arr, int n, int m, const char *filename)
{
  FILE *fp = fopen (filename, "r");
  if (fp != nullptr)
    {
      if (fread_matrix (fp, arr, n, m))
        {
          printf ("Can't read matrix from file %s\n", filename);
          fclose (fp);
          return !0;
        }
    }
  else
    {
      printf ("Can't open file %s\n", filename);
      return !0;
    }
  fclose (fp);
  return 0;
}

static void
st_init_square_matrix (double *arr, int n, int m, int s, double f (int, int, int, int))
{
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      arr[get_pos_bs (i, j, n, m)] = f (s, n, i, j);
}

static void
st_init_vec_b (double *A, double *b, int n, int m)
{
  for (int i = 0; i < n; i++)
    {
      b[i] = 0;
      for (int k = 0; k < (n + 1) / 2; k++)
        b[i] += A[get_pos_bs (i, 2 * k, n, m)];
    }
}

static void
st_init_vec_x (double *x, int n)
{
  for (int i = 0; i < n; i++)
    x[i] = (i + 1) % 2;
}

static double
f (int s, int n, int i, int j)
{
  i++, j++; // Formules were for i and j in range [1, n].
  switch (s)
    {
    case 1:
      return n - st_max (i, j) + 1;
    case 2:
      return st_max (i, j);
    case 3:
      return st_abs (i - j);
    case 4:
      return 1. / (i + j - 1);
    default:
      return (i == j) ? 1 : 0; // E.
    }
}

enum main_ret_codes
{
  Success = 0,
  Bad_input,
  Read_error,
  Memory_error
};

int
main (int argc, char *argv[])
{
  const int task = 16;
  int n, m, r, s, ret;
  char *filename;
  char libname[] = LIBNAME;
  double *A, *vec_b, *vec_b_res, *vec_x, *vec_buf;
  double *b1, *b2, *b3, *buf_vec, *buf_vec2;
  double t1 = 0, t2 = 0, r1 = -1, r2 = -1;
  clock_t t;
  double norma;

  // ./a.out n m r s [filename]
  if (!((argc == 5 || argc == 6) && (n = atoi (argv[1])) && (m = atoi (argv[2]))))
    {
      printf ("Usage: %s n m r (s | (0 filename))\n", argv[0]);
      return Bad_input;
    }
  r = atoi (argv[3]);
  s = atoi (argv[4]);
  if (m > n)
    {
      printf ("m > n => m -> n\n");
      m = n;
    }
  if (m % MICRO_DIMENSION != 0)
    m += MICRO_DIMENSION - m % MICRO_DIMENSION;

  { // Mem_allocation.
    A = (double *)_mm_malloc (sizeof (double) * n * n, INTRIN_SIZE);
    vec_b = (double *)_mm_malloc (sizeof (double) * n, INTRIN_SIZE);
    vec_x = (double *)_mm_malloc (sizeof (double) * n, INTRIN_SIZE);
    vec_buf = (double *)_mm_malloc (sizeof (double) * n, INTRIN_SIZE);
    vec_b_res = (double *)_mm_malloc (sizeof (double) * n, INTRIN_SIZE);
    b1 = (double *)_mm_malloc (sizeof (double) * m * m, INTRIN_SIZE);
    b2 = (double *)_mm_malloc (sizeof (double) * m * m, INTRIN_SIZE);
    b3 = (double *)_mm_malloc (sizeof (double) * m * m, INTRIN_SIZE);
    buf_vec = (double *)_mm_malloc (sizeof (double) * m * m, INTRIN_SIZE);
    buf_vec2 = (double *)_mm_malloc (sizeof (double) * m * m, INTRIN_SIZE);
  }

  // Initialization.
  if ((argc == 6) && (s == 0))
    {
      filename = argv[5];
      if (st_init_matrix (A, n, m, filename))
        {
          printf ("%s : Task = %d Res1 = %e Res2 = %e T1 = %.2f T2 = %.2f S = "
                  "%d N = %d M = %d\n",
                  argv[0], task, r1, r2, t1, t2, s, n, m);
          return Read_error;
        }
    }
  else
    st_init_square_matrix (A, n, m, s, f);
  st_init_vec_b (A, vec_b, n, m);
  memcpy (vec_b_res, vec_b, sizeof (double) * n);
  st_init_vec_x (vec_x, n);
  norma = calc_matrix_norm_inf (A, n, m);

  if (r)
    {
      printf ("Input matrix:\n");
      print_matrix_format_block (A, n, m, r);
      printf ("Vector b:\n");
      print_matrix_format (vec_b, 1, n, r);
    }

  t = clock ();
  // Same method (literally same .cpp file), but is compiled without agressive
  // optimizations.
  if (n <= 20)
    ret = accurate_solve_GJ (libname, n, m, norma, A, vec_b_res, b1, b2, b3, buf_vec, buf_vec2);
  else
    ret = solve_GJ (n, m, norma, A, vec_b_res, b1, b2, b3, buf_vec, buf_vec2);
  t = clock () - t;
  t1 = (double)t / CLOCKS_PER_SEC;
  // В vec_b_res лежит решение системы.

  printf ("---------------------\nOutput matrix:\n");
  print_matrix_format_block (A, n, m, r);
  if (r)
    {
      printf ("Vector res:\n");
      print_matrix_format (vec_b_res, 1, n, r);
    }

  if ((argc == 6) && (s == 0))
    {
      filename = argv[5];
      if (st_init_matrix (A, n, m, filename))
        return Read_error;
    }
  else
    st_init_square_matrix (A, n, m, s, f);
  t = clock ();
  r1 = calc_residual (A, vec_b_res, vec_b, vec_buf, n, m);
  r2 = calc_mistake (vec_x, vec_b_res, n);
  t = clock () - t;
  t2 = (double)t / CLOCKS_PER_SEC;

  if (ret)
    {
      r1 = -1;
      r2 = -1;
    }
  printf ("%s : Task = %d Res1 = %e Res2 = %e T1 = %.2f T2 = %.2f S = %d N = "
          "%d M = %d\n",
          argv[0], task, r1, r2, t1, t2, s, n, m);

  _mm_free (A);
  _mm_free (vec_b);
  _mm_free (vec_x);
  _mm_free (vec_buf);
  _mm_free (b1);
  _mm_free (b2);
  _mm_free (b3);
  _mm_free (buf_vec);
  _mm_free (buf_vec2);
  _mm_free (vec_b_res);
  return Success;
}
