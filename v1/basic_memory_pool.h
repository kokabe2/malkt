// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_BASIC_MEMORY_POOL_H_
#define V1_BASIC_MEMORY_POOL_H_

#include "memory_pool.h"

typedef struct {
  MemoryPool (*New)(void* memory_area, int capacity, int block_size);  //
} BasicMemoryPoolMethodStruct;
typedef const BasicMemoryPoolMethodStruct* BasicMemoryPoolMethod;

extern const BasicMemoryPoolMethod basicMemoryPool;

#endif  // V1_BASIC_MEMORY_POOL_H_
