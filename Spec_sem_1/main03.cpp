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

static void
st_print_type_seq (int type)
{
  enum
  {
    Indif_seq,
    Arith_seq,
    Geom_seq,
    Const_seq,
    Unknown_seq,
  };
  if (type == Indif_seq)
    printf ("Indifferent sequence\n");
  else if (type == Arith_seq)
    printf ("Arith. sequence\n");
  else if (type == Geom_seq)
    printf ("Geom. sequence\n");
  else if (type == Const_seq)
    printf ("Const. sequence\n");
  else if (type == Unknown_seq)
    printf ("Not enough elements\n");
}

int
main (int argc, char *argv[])
{
  int res;
  clock_t elapsed;

  if (!(argc == 2))
    return st_print_error_message (Bad_input_err, argv[0]);

  elapsed = clock ();
  res = tasks_get_seq_type (argv[1]);
  elapsed = clock () - elapsed;

  if (res < 0)
    {
      printf ("Read error %d\n", res);
      return res;
    }
  st_print_type_seq (res);
  printf ("Ret code %d\nElapsed %.2lf\n", res, (double)elapsed / CLOCKS_PER_SEC);
  return Success;
}