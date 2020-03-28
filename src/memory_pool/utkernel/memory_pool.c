﻿// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "memory_pool.h"

#include <stdbool.h>
#include <stddef.h>

#include "bleu/v1/heap.h"
#include "utkernel/utkernel.h"

typedef struct MemoryPoolStruct {
  int id;  //
} MemoryPoolStruct;

inline static void CreateMemoryPool(MemoryPool self, void* memory_area, int capacity, int block_size) {
  T_CMPF packet = {.mpfatr = TA_TFIFO | TA_RNG0 | TA_USERBUF,
                   .mpfcnt = capacity / block_size,
                   .blfsz = block_size,
                   .bufptr = memory_area};
  self->id = tk_cre_mpf(&packet);
}

static MemoryPool New(void* memory_area, int capacity, int block_size) {
  MemoryPool self = (MemoryPool)heap->New(sizeof(MemoryPoolStruct));
  CreateMemoryPool(self, memory_area, capacity, block_size);
  return self;
}

static void Delete(MemoryPool* self) {
  tk_del_mpf((*self)->id);
  heap->Delete((void**)self);
}

static void* Get(MemoryPool self) {
  void* block;
  return tk_get_mpf(self->id, &block, TMO_POL) == E_OK ? block : NULL;
}

static void Release(MemoryPool self, void* block) { tk_rel_mpf(self->id, block); }

static const MemoryPoolMethodStruct kTheMethod = {
    .New = New, .Delete = Delete, .Get = Get, .Release = Release,
};

const MemoryPoolMethod memoryPool = &kTheMethod;