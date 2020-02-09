// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_MEMORY_POOL_H_
#define V1_MEMORY_POOL_H_

typedef struct MemoryPoolStruct* MemoryPool;
typedef struct {
  MemoryPool (*New)(void* memory_area, int capacity, int block_size);
  void (*Delete)(MemoryPool* self);
  void* (*Get)(MemoryPool self);
  void (*Release)(MemoryPool self, void* block);
} MemoryPoolMethodStruct;
typedef const MemoryPoolMethodStruct* MemoryPoolMethod;

extern const MemoryPoolMethod memoryPool;

#endif  // V1_MEMORY_POOL_H_
