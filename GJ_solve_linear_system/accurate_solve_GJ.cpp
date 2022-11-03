#include <dlfcn.h>
#include <string.h>
#include <unistd.h>

#include "GJ_solve_core.cpp"

int
accurate_solve_GJ (char *libname, int n, int m, double norma, double *A, double *B_vec, double *b1, double *b2, double *b3, double *b_vec, double *b_vec2)
{
  const int LEN = 4096;
  char full_path_buf[LEN] = { '\0' };
  char full_path_buf2[LEN] = { '\0' };
  readlink ("/proc/self/exe", full_path_buf2, LEN);
  size_t end = strlen (full_path_buf2);
  for (; end > 0; end--)
    if ((full_path_buf2[end] == '/') && (end - 1 > 0) && (full_path_buf2[end] != '\\'))
      {
        full_path_buf2[end + 1] = '\0';
        break;
      }
  sprintf (full_path_buf, "%s%s", full_path_buf2, libname);
  // printf ("%s\n", full_path_buf);
  void *handle = dlopen (full_path_buf, RTLD_LAZY);
  if (handle == nullptr)
    {
      fprintf (stderr, "Can't load the library %s\n", full_path_buf);
      return solve_GJ (n, m, norma, A, B_vec, b1, b2, b3, b_vec, b_vec2);
    }

  void *ptrs[8] = { nullptr };
  ptrs[0] = (void *)mult_blocks_m;
  ptrs[1] = (void *)mult_blocks_m_sub;
  ptrs[2] = (void *)get_block;
  ptrs[3] = (void *)set_block;
  ptrs[4] = (void *)get_block_vec;
  ptrs[5] = (void *)set_block_vec;
  ptrs[6] = (void *)mult_block_on_vec;
  ptrs[7] = (void *)mult_block_on_vec_sub;

  // init_aux_functions ();
  mult_blocks_m = (void (*) (double *, double *, double *, int))dlsym (handle, "non_intrin_mult_blocks_m");
  mult_blocks_m_sub = (void (*) (double *, double *, double *, int))dlsym (handle, "non_intrin_mult_blocks_m_sub");
  get_block = (void (*) (double *, double *, int, int, int, int))dlsym (handle, "non_intrin_get_block");
  set_block = (void (*) (double *, double *, int, int, int, int))dlsym (handle, "non_intrin_set_block");
  get_block_vec = (void (*) (double *, double *, int, int, int))dlsym (handle, "non_intrin_get_block_vec");
  set_block_vec = (void (*) (double *, double *, int, int, int))dlsym (handle, "non_intrin_set_block_vec");
  mult_block_on_vec = (void (*) (double *, double *, double *, int))dlsym (handle, "non_intrin_mult_block_on_vec");
  mult_block_on_vec_sub = (void (*) (double *, double *, double *, int))dlsym (handle, "non_intrin_mult_block_on_vec_sub");

  int ret = solve_GJ (n, m, norma, A, B_vec, b1, b2, b3, b_vec, b_vec2, 0);
  dlclose (handle);

  mult_blocks_m = (void (*) (double *, double *, double *, int))ptrs[0];
  mult_blocks_m_sub = (void (*) (double *, double *, double *, int))ptrs[1];
  get_block = (void (*) (double *, double *, int, int, int, int))ptrs[2];
  set_block = (void (*) (double *, double *, int, int, int, int))ptrs[3];
  get_block_vec = (void (*) (double *, double *, int, int, int))ptrs[4];
  set_block_vec = (void (*) (double *, double *, int, int, int))ptrs[5];
  mult_block_on_vec = (void (*) (double *, double *, double *, int))ptrs[6];
  mult_block_on_vec_sub = (void (*) (double *, double *, double *, int))ptrs[7];

  return ret;
}
