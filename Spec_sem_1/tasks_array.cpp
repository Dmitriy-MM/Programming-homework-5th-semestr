#include <cstdio>

#include "tasks_array.h"

template <typename T>
static T
st_abs (T a)
{
  return a > 0 ? a : -a;
}

static const double eps = 1e-9;

// Task05.
static int
st_get_SCD (int a, int b) // Smallest common divisor.
{
  int tmp;
  while (a && b)
    {
      a = a % b;
      tmp = b;
      b = a;
      a = tmp;
    }
  return a;
}

void
tasks_cycle_right_shift (double *arr, int n, int shift)
{
  double tmp;
  int cycles;
  if (shift < 0)
    shift = n - (-shift) % n;
  else
    shift = shift % n;
  if (shift == 0)
    return;
  cycles = st_get_SCD (n, shift);
  for (int j = 0; j < cycles; j++)
    {
      tmp = arr[j];
      for (int i = 0; i < n / cycles - 1; i++)
        arr[(j + i * shift) % n] = arr[(j + (i + 1) * shift) % n];
      arr[(j + (n / cycles - 1) * shift) % n] = tmp;
    }
}

// Task06.
int
tasks_erase_lesser_x (double *arr, int n, double x)
{
  int i, j;
  for (i = 0, j = 0; i < n; i++)
    {
      if (arr[i] < x)
        continue;
      arr[j++] = arr[i];
    }
  return j;
}

// Task07.
int
tasks_sequence (const char *filename, double *arr, int n)
{
  int res = 0, i;
  double cur;

  FILE *fp = fopen (filename, "r");
  enum ret_codes
  {
    Success = 0,
    Open_file_err,
    Read_file_err,
  };
  if (fp == nullptr)
    return -Open_file_err;

  i = 0;
  while (fscanf (fp, "%lf", &cur) == 1)
    {
      if (st_abs (cur - arr[i]) < eps)
        i++;
      else
        {
          i = 0;
          if (st_abs (cur - arr[i]) < eps)
            i++;
        }
      if (n == i)
        res++, i = 0;
    }
  if (!feof (fp))
    {
      fclose (fp);
      return -Read_file_err;
    }
  fclose (fp);
  return res;
}

// Task08.
int
tasks_merge (double *a, int n, double *b, int m, double *c)
{
  int i = 0, j = 0, k = 0;
  while ((i < n) && (j < m))
    if ((a[i] < b[j]) || (a[i] > b[j]))
      c[k++] = a[i] < b[j] ? a[i++] : b[j++];
    else
      c[k++] = a[i++], j++;
  while (i < n)
    c[k++] = a[i++];
  while (j < m)
    c[k++] = b[j++];
  return n + m;
}