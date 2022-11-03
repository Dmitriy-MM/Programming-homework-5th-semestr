#ifndef INTRIN_AUX
#define INTRIN_AUX
void intrin_get_block (double *A_src, double *dst, int i, int j, int n, int m);
void intrin_set_block (double *A_dst, double *src, int i, int j, int n, int m);

void intrin_get_block_vec (double *A_src, double *dst, int i, int n, int m);
void intrin_set_block_vec (double *A_src, double *dst, int i, int n, int m);

void intrin_mult_blocks_m (double *A, double *B, double *C, int m);
void intrin_mult_block_on_vec (double *A, double *B, double *C, int m);

void intrin_mult_blocks_m_sub (double *A, double *B, double *C, int m);
void intrin_mult_block_on_vec_sub (double *A, double *B, double *C, int m);
#endif // INTRIN_AUX.
