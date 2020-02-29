// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "isr.h"

#include <stdbool.h>
#include <stddef.h>

#include "bleu/v1/heap.h"
#include "utkernel/utkernel.h"

typedef struct IsrStruct {
  int interrupt_number;  //
} IsrStruct;

inline static void Register(Isr self, InterruptDelegate interrupt) {
  T_DINT packet = {.intatr = TA_HLNG, .inthdr = (FP)interrupt};
  tk_def_int((UINT)self->interrupt_number, &packet);
}

static Isr New(int interrupt_number, InterruptDelegate interrupt) {
  Isr self = (Isr)heap->New(sizeof(IsrStruct));
  self->interrupt_number = interrupt_number;
  Register(self, interrupt);
  return self;
}

static void InterruptDummy(int unused) {}

inline static void Unregister(Isr self) { Register(self, InterruptDummy); }

static void Delete(Isr* self) {
  DisableInt((UINT)(*self)->interrupt_number);
  Unregister(*self);
  heap->Delete((void**)self);
}

static void Enable(Isr self, int level) { EnableInt((UINT)self->interrupt_number, level); }

static void Disable(Isr self) { DisableInt((UINT)self->interrupt_number); }

static const IsrMethodStruct kTheMethod = {
    .New = New, .Delete = Delete, .Enable = Enable, .Disable = Disable,
};

const IsrMethod isr = &kTheMethod;
