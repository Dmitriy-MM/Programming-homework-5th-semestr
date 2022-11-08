#ifndef NAN_DOUBLE
#define NAN_DOUBLE

#include <stdarg.h>
#include <fenv.h>

static void nan_d (int count, ...)
{
  (void) nan_d;
  feenableexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW);
  const long long int NaN = 0x7ff07ff07ff07ff0;
  va_list list;
  va_start(list, count);
  for (int i = 0; i < count/2; i++)
    {
      double * d_arg = static_cast<double *> (va_arg (list, double *));
      int d_len = static_cast<int> (va_arg (list, int));
      for (int j = 0; j < d_len; j++)
        *((long long int *)( (void *)(d_arg + j) )) = NaN;
    }
  va_end(list);
}
#endif // NAN_DOUBLE
