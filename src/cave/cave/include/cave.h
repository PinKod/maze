#ifndef CAVE_H
#define CAVE_H

#include "./../../../includes/defines.h"

typedef long long int lli;

typedef struct {
  size_t rows;
  size_t cols;
  unsigned char** map;
} cave;

// cave allocators
cave* new_cave(size_t rows, size_t cols);
void free_cave(cave* cave);

// cave generate functions
int update_cave(cave* cave, size_t birth_limit, size_t death_limit);

#endif  // CAVE_H