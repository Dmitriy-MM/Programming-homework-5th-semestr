#ifndef TASKS_SEQUENCES
#define TASKS_SEQUENCES

int tasks_min_distance (const char *filename, int k, int *is_found);
int tasks_check_2_seqs (const char *filename_a, const char *filename_b);
int tasks_get_seq_type (const char *filename_a);
int tasks_get_standard_deviation (const char *filename, double *destination);

#endif // TASKS_SEQUENCES