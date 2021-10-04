#include <cmath>
#include <cstdio>

#include "tasks_sequences.h"
// #define DBG

static const int bits_per_byte = 8;
static const double eps = 1e-9;

template <typename T>
static T
st_abs (T a)
{
  return a > 0 ? a : -a;
}

// Task01.
static void
st_get_bits (char *arr, int len, int num)
{
  for (int i = 0; i < len; i++)
    arr[len - i - 1] = num & (1 << i) ? 1 : 0;
#ifdef DBG
  for (int i = 0; i < len; i++)
    printf ("%d", (int)arr[i]);
  printf ("\n");
#endif
}

int
tasks_min_distance (const char *filename, int k, int *is_found)
{
  int cur, distance_count, distance_min, ones_in_a_row;
  const int Len = sizeof (int) * bits_per_byte;
  char num_container[Len] = { 0 };
  FILE *fp;
  enum Ret_codes
  {
    Success = 0,
    Open_file_err,
    Read_err,
  };

  fp = fopen (filename, "r");
  if (fp == nullptr)
    return -Open_file_err;
  *is_found = 0;
  ones_in_a_row = 0, distance_count = -1, distance_min = -1;
  while (fscanf (fp, "%d", &cur) == 1)
    {
      st_get_bits (num_container, Len, cur);
      for (int i = 0; i < Len; i++)
        {
          ones_in_a_row = num_container[i] ? ones_in_a_row + 1 : 0;
          distance_count = distance_count < 0 ? distance_count : distance_count + 1;
          if (ones_in_a_row == k)
            {
              if (distance_count >= 0)
                {
                  if (distance_min < 0)
                    distance_min = distance_count - k, *is_found = 1;
                  else
                    distance_min
                        = (distance_count - k) < distance_min ? distance_count - k : distance_min;
                }
              ones_in_a_row = 0, distance_count = 0;
            }
        }
    }
  if (!feof (fp))
    {
      fclose (fp);
      return -Read_err;
    }
  fclose (fp);
  if (*is_found)
    return distance_min;
  else
    return Success;
}

// Task02.
int
tasks_check_2_seqs (const char *filename_a, const char *filename_b)
{
  FILE *fp_a, *fp_b;
  double a1, a2, a3, b;
  int c1, c2;
  enum Ret_codes
  {
    Success = 0,
    Open_file_err,
    Read_err,
  };
  fp_a = fopen (filename_a, "r");
  if (fp_a == nullptr)
    return -Open_file_err;
  fp_b = fopen (filename_b, "r");
  if (fp_b == nullptr)
    {
      fclose (fp_a);
      return -Open_file_err;
    }

  if (fscanf (fp_a, "%lf%lf", &a1, &a2) != 2)
    {
      fclose (fp_a), fclose (fp_b);
      return -Read_err;
    }
  c1 = (fscanf (fp_b, "%lf", &b) == 1), c2 = (fscanf (fp_a, "%lf", &a3) == 1);
  while (c1 && c2)
    {
      if (st_abs (b - (a1 + a3) / 2) > eps)
        {
          fclose (fp_a), fclose (fp_b);
          return 0;
        }
      a1 = a2;
      a2 = a3;
      c1 = (fscanf (fp_b, "%lf", &b) == 1), c2 = (fscanf (fp_a, "%lf", &a3) == 1);
    }
  if (!feof (fp_b) || (c1 && !c2))
    {
      fclose (fp_a), fclose (fp_b);
      return -Read_err;
    }
  fclose (fp_a), fclose (fp_b);
  return 1;
}

// Task03.
int
tasks_get_seq_type (const char *filename)
{
  FILE *fp;
  double cur, prev, next;
  int ret, seq_type, is_arith, is_geom;
  enum Ret_codes
  {
    Success = 0,
    Arith_seq,
    Geom_seq,
    Const_seq,
    Unknown_seq,
    Open_file_err,
    Read_err,
  };
  fp = fopen (filename, "r");
  if (fp == nullptr)
    return -Open_file_err;

  ret = fscanf (fp, "%lf%lf%lf", &prev, &cur, &next);
  if (ret < 3)
    {
      if (!feof (fp))
        return Read_err;
      fclose (fp);
      return Unknown_seq;
    }
  seq_type = 0xF;
  do
    {
      is_arith = st_abs (next - (2 * cur - prev)) < eps ? 1 : 0;
      is_geom = st_abs (next - (cur * cur / prev)) < eps ? 1 : 0;
      seq_type &= (is_arith + (is_geom << 1));
      prev = cur;
      cur = next;
    }
  while (fscanf (fp, "%lf", &next) == 1);
  if (!feof (fp))
    {
      fclose (fp);
      return -Read_err;
    }
  fclose (fp);
  return seq_type;
}

// Task04.
int
tasks_get_standard_deviation (const char *filename, double *destination)
{
  FILE *fp;
  double cur, sum_quares = 0, sum_arith = 0;
  int n = 0;
  enum Ret_codes
  {
    Success = 0,
    Open_file_err,
    Read_err,
  };

  fp = fopen (filename, "r");
  if (fp == nullptr)
    return -Open_file_err;

  while (fscanf (fp, "%lf", &cur) == 1)
    {
      sum_quares += cur * cur;
      sum_arith += cur;
      n++;
    }
  if (!feof (fp))
    {
      fclose (fp);
      return -Read_err;
    }
  fclose (fp);
  *destination = sqrt ((sum_quares - sum_arith * sum_arith / n) / n);
  return Success;
}