// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "basic_memory_pool.h"

#include <stdbool.h>
#include <stddef.h>

#include "bleu/v1/heap.h"
#include "utkernel/utkernel.h"

typedef struct {
  MemoryPoolInterfaceStruct impl;
  int id;
} BasiceMemoryPoolStruct, *BasiceMemoryPool;

static void Delete(MemoryPool* self) {
  tk_del_mpf(((BasiceMemoryPool)(*self))->id);
  heap->Delete((void**)self);
}

static void* Get(MemoryPool self) {
  void* block;
  return tk_get_mpf(((BasiceMemoryPool)self)->id, &block, TMO_POL) == E_OK ? block : NULL;
}

static void Release(MemoryPool self, void* block) { tk_rel_mpf(((BasiceMemoryPool)self)->id, block); }

static const MemoryPoolInterfaceStruct kTheInterface = {
    .Delete = Delete, .Get = Get, .Release = Release,
};

inline static void CreateMemoryPool(BasiceMemoryPool self, void* memory_area, int capacity, int block_size) {
  T_CMPF packet = {.mpfatr = TA_TFIFO | TA_RNG0 | TA_USERBUF,
                   .mpfcnt = capacity / block_size,
                   .blfsz = block_size,
                   .bufptr = memory_area};
  self->id = tk_cre_mpf(&packet);
}

static MemoryPool New(void* memory_area, int capacity, int block_size) {
  BasiceMemoryPool self = (BasiceMemoryPool)heap->New(sizeof(BasiceMemoryPoolStruct));
  self->impl = kTheInterface;
  CreateMemoryPool(self, memory_area, capacity, block_size);
  return (MemoryPool)self;
}

static const BasicMemoryPoolMethodStruct kTheMethod = {
    .New = New,
};

const BasicMemoryPoolMethod basicMemoryPool = &kTheMethod;
