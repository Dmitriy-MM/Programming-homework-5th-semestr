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
      printf ("Usage: %s filename_1 filename_2\n", s);
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
  int res;
  clock_t elapsed;

  if (!(argc == 3))
    return st_print_error_message (Bad_input_err, argv[0]);

  elapsed = clock ();
  res = tasks_check_2_seqs (argv[1], argv[2]);
  elapsed = clock () - elapsed;

  if (res < 0)
    {
      printf ("Read error %d\n", res);
      return res;
    }
  printf ("Ret code %d\nElapsed %.2lf\n", res, (double)elapsed / CLOCKS_PER_SEC);
  return Success;
}