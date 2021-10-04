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
      printf ("Usage: %s n p_a (s_a | (0 filename_a)) m p_b (s_b | (0 "
              "filename_b))",
              s);
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
  int n, p_a, s_a, m, p_b, s_b;
  int i, ret_code;
  const char *filename_a = nullptr, *filename_b = nullptr;
  std::unique_ptr<double[]> a, b, c;
  clock_t elapsed;

  if (!((argc >= 7) && (argc <= 9)))
    return st_print_error_message (Bad_input_err, argv[0]);
  i = 1;
  n = atoi (argv[i++]);
  p_a = atoi (argv[i++]);
  s_a = atoi (argv[i++]);
  if (s_a == 0)
    {
      if (argc >= 8)
        filename_a = argv[i++];
      else
        return st_print_error_message (Bad_input_err, argv[0]);
    }
  m = atoi (argv[i++]);
  p_b = atoi (argv[i++]);
  s_b = atoi (argv[i++]);
  if (s_b == 0)
    {
      if (argc >= i)
        filename_b = argv[i++];
      else
        return st_print_error_message (Bad_input_err, argv[0]);
    }
  a = std::make_unique<double[]> (n);
  b = std::make_unique<double[]> (m);
  c = std::make_unique<double[]> (n + m);
  if ((a.get () == nullptr) || (b.get () == nullptr) || (c.get () == nullptr))
    return st_print_error_message (Mem_err);

  ret_code = st_read_array (a.get (), n, s_a, filename_a);
  if (ret_code)
    return st_print_error_message (ret_code);
  ret_code = st_read_array (b.get (), m, s_b, filename_b);
  if (ret_code)
    return st_print_error_message (ret_code);

  printf ("A:\n");
  locar_print_array (a.get (), n, p_a);
  printf ("B:\n");
  locar_print_array (b.get (), m, p_b);

  elapsed = clock ();
  tasks_merge (a.get (), n, b.get (), m, c.get ());
  elapsed = clock () - elapsed;

  printf ("\nC:\n");
  locar_print_array (c.get (), n + m, p_a + p_b);
  printf ("Elapsed %.2lf\n", (double)elapsed / CLOCKS_PER_SEC);
  return Success;
}