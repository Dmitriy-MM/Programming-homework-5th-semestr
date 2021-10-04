#include <cstdio>

#include "local_array.h"

static double
st_f (int s, int n, int i)
{
  switch (s)
    {
    case 1:
      return i;
    case 2:
      return n - i;
    case 3:
      return i / 2;
    case 4:
      return n - i / 2;
    case 5:
      return 2 * i;
    case 6:
      return n - 2 * i;
    default:
      break;
    }
  return 0;
}

int
locar_read_array (double *arr, int dimension, FILE *fp)
{
  int i;
  for (i = 0; i < dimension; i++)
    if ((fscanf (fp, "%lf", arr + i) != 1) && (!feof (fp)))
      break;
  return i;
}

void
locar_print_array (double *arr, int dimension, int print_max, FILE *fp)
{
  print_max = print_max > dimension ? dimension : print_max;
  for (int i = 0; i < print_max; i++)
    fprintf (fp, "%2d) %lf\n", i, arr[i]);
}

void
locar_init_array (double *arr, int dimension, int formula)
{
  for (int i = 0; i < dimension; i++)
    arr[i] = st_f (formula, dimension, i);
}