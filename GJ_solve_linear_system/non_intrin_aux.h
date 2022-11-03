#ifndef NON_INTRIN_AUX
#define NON_INTRIN_AUX
extern "C"
{
  void non_intrin_get_block (double *A_src, double *dst, int i, int j, int n, int m);
  void non_intrin_set_block (double *A_dst, double *src, int i, int j, int n, int m);

  void non_intrin_get_block_vec (double *A_src, double *dst, int i, int n, int m);
  void non_intrin_set_block_vec (double *A_src, double *dst, int i, int n, int m);

  void non_intrin_mult_blocks_m (double *A, double *B, double *C, int m);
  void non_intrin_mult_block_on_vec (double *A, double *B, double *C, int m);

  void non_intrin_mult_blocks_m_sub (double *A, double *B, double *C, int m);
  void non_intrin_mult_block_on_vec_sub (double *A, double *B, double *C, int m);
}
#endif // NON_INTRIN_AUX.
