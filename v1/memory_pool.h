// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_MEMORY_POOL_H_
#define V1_MEMORY_POOL_H_

typedef struct MemoryPoolStruct* MemoryPool;
typedef struct {
  void (*Delete)(MemoryPool* self);
  void* (*Get)(MemoryPool self);
  void (*Release)(MemoryPool self, void* block);
} MemoryPoolInterfaceStruct;
typedef const MemoryPoolInterfaceStruct* MemoryPoolInterface;

extern const MemoryPoolInterface memoryPool;

#endif  // V1_MEMORY_POOL_H_
