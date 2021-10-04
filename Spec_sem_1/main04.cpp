#include <cstdio>
#include <ctime>
#include <memory>

#include "tasks_sequences.h"

enum Main_ret_codes
{
  Success = 0,
  Bad_input_err,
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
      printf ("Usage: %s filename\n", s);
      break;
    default:
      printf ("Unknown error %d\n", er);
      break;
    }
  return er;
}

int
main (int argc, char *argv[])
{
  int ret;
  double res;
  clock_t elapsed;

  if (!(argc == 2))
    return st_print_error_message (Bad_input_err, argv[0]);

  elapsed = clock ();
  ret = tasks_get_standard_deviation (argv[1], &res);
  elapsed = clock () - elapsed;

  if (ret < 0)
    {
      printf ("Read error %d\n", ret);
      return res;
    }
  printf ("Standard deviation %lf\nElapsed %.2lf\n", res, (double)elapsed / CLOCKS_PER_SEC);
  return Success;
}