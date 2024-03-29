#include <cmath>
#include <cstdio>
#include <memory>
#include <cstring>
#include <time.h>

#include "GJ_solve.h"
#include "matrix.h"
#include "aux_matrix_funcs.h"
#include "block_storage_aux_funcs.h"

// В помеченных (*) циклах можно не работать со столбцом на диагонали,
// так как он больше не нужен в решении, поэтому старт начинается с
// индекса + 1, но можно плюс убрать - тогда ответ не изменится, программа
// замедлится, но на вывод будет хорошая единичная матрица, которая
// подразумевается методом.
#define FORCE_SKIPING_DIAG (1)
// С выбором главного элемента
#define CHOOSING

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
          if (k >= dim)
            {
              //fprintf (stderr, "Can't reverse eps=%e, k = %d\n", eps, k);
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
solve_GJ (int n, int m, double norma, double *A, double *B_vec, double *vec_buf, double *b1, double *b2, double *b3, double *b_vec, double *b_vec2, int * seq, int void_arg)
{
  // Если сделать 1e-17 начнет обращать вырожденные матрицы.
  const double eps = norma * 1e-16; 
  int k = n / m;
  int r = n % m;
  int i, j, l;
  int ret;
  (void)void_arg;
  (void)vec_buf;
  clock_t t, tt = 0;

  if (r)
    k++;
  for (i = 0; i < k; i++)
    {

      // Выбор главного элемента по строке.
#ifdef CHOOSING
      double min_norm = norma + 1;
      int dim = n / m;
#endif
      int min = i;

      get_block (A, b1, i, i, n, m);
      if (r && (k - 1 == i))
        ret = reverse (b1, b2, m, r, eps);
      else
        ret = reverse (b1, b2, m, eps);

#ifdef CHOOSING
      if (!ret)
        min_norm = calc_matrix_norm_inf (b2, m, m);
      for (j = i + 1; j < dim; j++)
        {
          int ret_in;
          get_block (A, b1, i, j, n, m);
          ret_in = reverse (b1, b3, m, eps);
          if (ret_in)
            continue;
          double norm = calc_matrix_norm_inf (b3, m, m);
          if (norm <= min_norm)
            {
              min_norm = norm;
              memcpy (b2, b3, sizeof (double) * m * m);
              min = j;
              ret = ret_in;
            }
        }
#endif
      if (ret)
        {
          printf ("Can't solve via this method\n");
          return ret;
        }
      if (i != min)
        {
          // Перестановка столбцов.
          int t = seq[i];
          seq[i] = seq[min];
          seq[min] = t;

          // Дальше циклы раскрыты на цикл до k-1 и последующий if, чтобы, когда
          // лежит целый блок m*m не копировать его в буфер, а работать прямо в
          // памяти с матрицей.
          for (l = 0; l < k - 1; l++)
            {
              double * A_li = A + get_shift_via_block_coords (l, i, k, n, m);
              double * A_lmin = A + get_shift_via_block_coords (l, min, k, n, m);
              memcpy (b1, A_li, sizeof (double) * m * m); // сохранили li
              memcpy (A_li, A_lmin, sizeof (double) * m * m); // записали lmin на li
              memcpy (A_lmin, b1, sizeof (double) * m * m); // записали li на lmin
            }
          if (l < k)
            {
              get_block (A, b1, l, i, n, m);
              get_block (A, b3, l, min, n, m);
              set_block (A, b3, l, i, n, m);
              set_block (A, b1, l, min, n, m);
            } // for (l = 0; l < k - 1; l++)
        }

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
              get_block (A, b2, i, l, n, m); // вычитаемая строка без домножения
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
#ifdef CHOOSING
  // Восстановление вектора после перестановок столбцов.
  for (i = 0; i < k; i++)
  {
    get_block_vec (B_vec, b_vec, i, n, m);
    set_block_vec (b_vec, vec_buf, seq[i], n, m);
  }
  memcpy (B_vec, vec_buf, sizeof(double) * n);
#endif
  fprintf (stderr, "mult_blocks_m & mult_blocks_m_sub elapsed time: %.2lfs\n", (double)tt / CLOCKS_PER_SEC);
  return 0;
}
