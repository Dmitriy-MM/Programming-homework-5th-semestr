#ifndef AUX_MATRIX_FUNCS
#define AUX_MATRIX_FUNCS

extern void (*get_block) (double *A_src, double *dst, int i, int j, int n, int m);
extern void (*set_block) (double *A_dst, double *src, int i, int j, int n, int m);

extern void (*get_block_vec) (double *A_src, double *dst, int i, int n, int m);
extern void (*set_block_vec) (double *A_src, double *dst, int i, int n, int m);

extern void (*mult_blocks_m) (double *A, double *B, double *C, int m);
extern void (*mult_block_on_vec) (double *A, double *B, double *C, int m);

extern void (*mult_blocks_m_sub) (double *A, double *B, double *C, int m);
extern void (*mult_block_on_vec_sub) (double *A, double *B, double *C, int m);

void init_aux_functions (int do_force_non_itrin = DO_FORCE_NON_INTRIN);
#endif // AUX_MATRIX_FUNCS.
