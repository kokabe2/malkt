// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_MEMORY_POOL_MEMORY_POOL_PRIVATE_H_
#define SRC_MEMORY_POOL_MEMORY_POOL_PRIVATE_H_

#include "memory_pool.h"

typedef struct MemoryPoolStruct {
  MemoryPoolInterface impl;
  int id;
} MemoryPoolStruct;

#endif  // SRC_MEMORY_POOL_MEMORY_POOL_PRIVATE_H_
