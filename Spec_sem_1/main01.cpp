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
      printf ("Usage: %s seq_filename k\n", s);
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
  int res, k, is_found;
  clock_t elapsed;

  if (!(argc == 3))
    return st_print_error_message (Bad_input_err, argv[0]);
  k = atoi (argv[2]);

  elapsed = clock ();
  res = tasks_min_distance (argv[1], k, &is_found);
  elapsed = clock () - elapsed;

  if (res < 0)
    {
      printf ("Read error %d\n", res);
      return res;
    }
  if (!is_found)
    printf ("Can't find two sequences of bits with length = %d\n", k);
  printf ("Min distance = %d\nElapsed %.2lf\n", res, (double)elapsed / CLOCKS_PER_SEC);
  return Success;
}