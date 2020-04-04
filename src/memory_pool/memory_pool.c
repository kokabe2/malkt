// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "memory_pool.h"

static void Delete(MemoryPool* self) { (*self)->Delete(self); }

static void* Get(MemoryPool self) { return self->Get(self); }

static void Release(MemoryPool self, void* block) { self->Release(self, block); }

static const MemoryPoolInterfaceStruct kTheInterface = {
    .Delete = Delete, .Get = Get, .Release = Release,
};

const MemoryPoolInterface memoryPool = &kTheInterface;
