#include <cmath>
#include <cstdio>
#include <memory>
#include <time.h>

#include "GJ_solve.h"
#include "aux_matrix_funcs.h"
#include "block_storage_aux_funcs.h"

// В помеченных (*) циклах можно не работать со столбцом на диагонали,
// так как он больше не нужен в решении, поэтому старт начинается с
// индекса + 1, но можно плюс убрать - тогда ответ не изменится, программа
// замедлится, но на вывод будет хорошая матрица, которая подразумевается
// методом.
#define FORCE_SKIPING_DIAG (1)

static void
erase_vec (double *A, int n)
{
  for (int i = 0; i < n; i++)
    A[i] = 0;
}

static void
init_E_matrix (double *A, int n, int dim = -1)
{
  dim = dim < 0 ? n : dim;
  for (int i = 0; i < n * dim; i++)
    A[i] = 0;
  for (int i = 0; i < dim; i++)
    A[i * (n + 1)] = 1;
}

// Нахождение обратной матрицы, А - исходная, B - присоединенная.
static int
reverse (double *A, double *B, int n, int dim, double eps = 1e-14)
{
  // Через присоединенную матрицу находим обратную.
  // Возвращаем !0, если не удалось обратить матрицу
  // (главный элемент 0/матрица вырождена).
  init_E_matrix (B, n, dim);
  for (int i = 0; i < dim; i++)
    {

      if (fabs (A[i * (n + 1)]) > eps)
        {
          double c = A[i * (n + 1)];
          for (int j = 0; j < n; j++)
            A[i * n + j] /= c, B[i * n + j] /= c;
        }
      else
        {
          int k;
          for (k = i + 1; k < dim; k++)
            if (abs (A[k * n + i]) > eps)
              {
                for (int s = 0; s < n; s++)
                  A[i * n + s] += A[k * n + s], B[i * n + s] += B[k * n + s];
                break;
              }
          if (k == dim)
            {
              fprintf (stderr, "Can't reverse eps=%e, k = %d\n", eps, k);
              return !0;
            }
          i--;
          continue;
        }
      for (int k = 0; k < dim; k++)
        {
          double c = A[k * n + i];
          if (k == i)
            continue;
          for (int j = 0; j < n; j++)
            {
              A[k * n + j] -= A[i * n + j] * c, B[k * n + j] -= B[i * n + j] * c;
            }
        }
    }
  return 0;
}

// Нахождение обратной матрицы, А - исходная, B - присоединенная.
static int
reverse (double *A, double *B, int n, double eps)
{
  return reverse (A, B, n, n, eps);
}

static int
solve_GJ (int n, int m, double norma, double *A, double *B_vec, double *b1, double *b2, double *b3, double *b_vec, double *b_vec2, int void_arg)
{
  const double eps = norma * 1e-14;
  int k = n / m;
  int r = n % m;
  int i, j, l;
  int ret;
  (void)erase_vec;
  (void)void_arg;
  clock_t t, tt = 0;

  if (r)
    k++;
  for (i = 0; i < k; i++)
    {
      get_block (A, b1, i, i, n, m);
      // Обращение диагонального элемента
      if (r && (k - 1 == i))
        ret = reverse (b1, b2, m, r, eps);
      else
        ret = reverse (b1, b2, m, eps);

      if (ret)
        {
          printf ("Can't solve via this method\n");
          return ret;
        }
      // Дальше циклы раскрыты на цикл до k-1 и последующий if, чтобы, когда
      // лежит целый блок m*m не копировать его в буфер, а работать прямо в
      // памяти с матрицей.

      // В помеченных (*) циклах можно не работать со столбцом на диагонали,
      // так как он больше не нужен в решении, поэтому старт начинается с
      // индекса + 1, но можно плюс убрать - тогда ответ не изменится,
      // программа замедлится, но на вывод будет хорошая матрица, которая
      // подразумевается методом. Домножение строки на обращенный диагональный
      // элемент
      for (j = i + FORCE_SKIPING_DIAG; j < k - 1; j++) // (*)
        {
          double *A_ij = A + get_shift_via_block_coords (i, j, k, n, m);
          get_block (A, b1, i, j, n, m);
          t = clock ();
          mult_blocks_m (b2, b1, A_ij, m);
          t = clock () - t;
        }
      if (j < k)
        {
          get_block (A, b1, i, j, n, m);
          t = clock ();
          mult_blocks_m (b2, b1, b3, m);
          t = clock () - t;
          set_block (A, b3, i, j, n, m);
        } // for (j = i; j < k-1; j++)

      get_block_vec (B_vec, b_vec, i, n, m);
      mult_block_on_vec (b2, b_vec, b_vec2, m);
      set_block_vec (b_vec2, B_vec, i, n, m);

      // Вычитание из вышележащих и нижележащих строк.
      for (j = 0; j < k - 1; j++)
        {
          if (j == i)
            continue;
          get_block (A, b3, j, i, n,
                     m);                                   // первый элемент строки из которой вычитается
          for (l = i + FORCE_SKIPING_DIAG; l < k - 1; l++) // (*)
            {
              // Внешний и внутренний циклы идут до k-1, т.е. будут только
              // целые блоки можно вообще не копировать в буфер, а просто
              // работать с памятью внутри матрицы. A_jl -= A_ji*A_il

              double *A_il = A + get_shift_via_block_coords (i, l, k, n, m);
              double *A_jl = A + get_shift_via_block_coords (j, l, k, n, m);
              t = clock ();
              mult_blocks_m_sub (b3, A_il, A_jl, m);
              t = clock () - t;
              tt += t;
            }
          if (l < k)
            {
              get_block (A, b1, j, l, n, m); // строка из которой вычитается
              get_block (A, b2, i, l, n,
                         m); // вычитаемая строка без домножения
              t = clock ();
              mult_blocks_m_sub (b3, b2, b1, m);
              t = clock () - t;
              tt += t;
              set_block (A, b1, j, l, n, m);
            } // for (l = i; l < k-1; l++)

          get_block_vec (B_vec, b_vec2, i, n, m);
          get_block_vec (B_vec, b_vec, j, n, m);
          mult_block_on_vec_sub (b3, b_vec2, b_vec, m);
          set_block_vec (b_vec, B_vec, j, n, m);
        }
      if (j < k)
        {
          if (j == i)
            continue;
          get_block (A, b3, j, i, n,
                     m);                               // первый элемент строки из которой вычитается
          for (l = i + FORCE_SKIPING_DIAG; l < k; l++) // (*)
            {
              get_block (A, b1, j, l, n, m); // строка из которой вычитается
              get_block (A, b2, i, l, n,
                         m); // вычитаемая строка без домножения
              t = clock ();
              mult_blocks_m_sub (b3, b2, b1, m);
              t = clock () - t;
              tt += t;
              set_block (A, b1, j, l, n, m);
            }
          get_block_vec (B_vec, b_vec2, i, n, m);
          get_block_vec (B_vec, b_vec, j, n, m);
          mult_block_on_vec_sub (b3, b_vec2, b_vec, m);
          set_block_vec (b_vec, B_vec, j, n, m);
        } // for (j = 0; j < k - 1; j++)
    }
  fprintf (stderr, "mult_blocks_m & mult_blocks_m_sub elapsed time: %.2lfs\n", (double)tt / CLOCKS_PER_SEC);
  return 0;
}
