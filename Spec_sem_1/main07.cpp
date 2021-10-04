#include <cstdio>
#include <ctime>
#include <memory>

#include "local_array.h"
#include "tasks_array.h"

enum Main_ret_codes
{
  Success = 0,
  Bad_input_err,
  Mem_err,
  Open_file_err,
  Read_array_err,
};

static int
st_print_error_message (int er, const char *s = nullptr)
{
  switch (er)
    {
    case Success:
      break;
    case Bad_input_err:
      s = s == nullptr ? s : "./a.out";
      printf ("Usage: %s x n p s arr_filename\n", s);
      break;
    case Mem_err:
      printf ("Memory allocation error\n");
      break;
    case Open_file_err:
      s = s == nullptr ? "" : s;
      printf ("Can't open file %s", s);
      break;
    case Read_array_err:
      s = s == nullptr ? "" : s;
      printf ("Can't read array from file %s", s);
      break;
    default:
      printf ("Unknown error %d\n", er);
      break;
    }
  return er;
}

static int
st_read_array (double *a, int n, int s_a, const char *filename_a)
{
  if (s_a == 0)
    {
      FILE *fp = fopen (filename_a, "r"); // If filename == nullptr then
                                          // fault be better than nothing.
      int ret_code;
      if (fp == nullptr)
        return st_print_error_message (Open_file_err, filename_a);
      ret_code = locar_read_array (a, n, fp);
      fclose (fp);
      if (ret_code != n)
        return st_print_error_message (Read_array_err, filename_a);
    }
  else
    locar_init_array (a, n, s_a);
  return 0;
}

int
main (int argc, char *argv[])
{
  int n, p, s;
  int i, ret_code;
  double x;
  const char *arr_filename = nullptr;
  std::unique_ptr<double[]> a;
  clock_t elapsed;

  if (!((argc >= 5) && (argc <= 6)))
    return st_print_error_message (Bad_input_err, argv[0]);
  i = 1;
  if (sscanf (argv[i++], "%lf", &x) != 1)
    return st_print_error_message (Bad_input_err, argv[0]);
  n = atoi (argv[i++]);
  p = atoi (argv[i++]);
  s = atoi (argv[i++]);
  if (s == 0)
    {
      if (argc >= 6)
        arr_filename = argv[i++];
      else
        return st_print_error_message (Bad_input_err, argv[0]);
    }

  a = std::make_unique<double[]> (n);
  if (a.get () == nullptr)
    return st_print_error_message (Mem_err);

  ret_code = st_read_array (a.get (), n, s, arr_filename);
  if (ret_code)
    return st_print_error_message (ret_code);

  printf ("A:\n");
  locar_print_array (a.get (), n, p);

  elapsed = clock ();
  ret_code = tasks_erase_lesser_x (a.get (), n, x);
  elapsed = clock () - elapsed;

  printf ("\nA after erasing (x = %lf):\n", x);
  locar_print_array (a.get (), ret_code, p);

  if (ret_code < 0)
    printf ("Read error\n");

  printf ("New length %d\nElapsed %.2lf\n", ret_code, (double)elapsed / CLOCKS_PER_SEC);
  return Success;
}