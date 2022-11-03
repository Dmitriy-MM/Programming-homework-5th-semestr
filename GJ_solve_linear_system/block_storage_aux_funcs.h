#ifndef BLOCK_STORAGE_AUX_FUNCS
#define BLOCK_STORAGE_AUX_FUNCS

// int get_shift_via_block_coords (int i, int j, int k, int n, int m);

// get position in block storage
// int get_pos_bs (int i, int j, int n, int m);
inline static int
get_shift_via_block_coords (int i, int j, int k, int n, int m)
{
  int block_r = ((k * m != n) && (i == k - 1)) ? n % m : m;
  int shift_block = (i * n + j * block_r) * m;
  return shift_block;
}

// get position in block storage
inline static int
get_pos_bs (int i, int j, int n, int m)
{
  int r = n % m;
  int num_of_blocks = n / m + (r ? 1 : 0);
  int block_i = i / m;
  int block_j = j / m;
  int block_r = (r && (block_i == num_of_blocks - 1)) ? r : m;
  int shift_block = (block_i * n + block_j * block_r) * m;
  int pos_r = (r && (block_j == num_of_blocks - 1)) ? r : m;
  int pos = shift_block + (i % m) * pos_r + (j % m);
  return pos;
}
#endif // BLOCK_STORAGE_AUX_FUNCS
