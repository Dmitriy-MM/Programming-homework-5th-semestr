#ifndef TASKS_ARRAY
#define TASKS_ARRAY

int tasks_merge (double *a, int n_a, double *b, int n_b, double *c);
int tasks_sequence (const char * filename, double * arr, int n);
int tasks_erase_lesser_x (double * arr, int n, double x);
void tasks_cycle_right_shift (double * arr, int n, int shift);

#endif // TASKS_ARRAY