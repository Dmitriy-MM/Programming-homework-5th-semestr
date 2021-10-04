#ifndef LOCAL_ARRAY
#define LOCAL_ARRAY

int locar_read_array (double *arr, int dimension, FILE *fp = stdin);
void locar_print_array (double *arr, int dimension, int print_max, FILE *fp = stdout);
void locar_init_array (double *arr, int dimension, int formula);

#endif // LOCAL_ARRAY