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

inline static bool Validate(int interrupt_number, InterruptDelegate interrupt) {
  return interrupt_number >= 0 && interrupt;
}
inline static bool Register(Isr self, InterruptDelegate interrupt) {
  T_DINT packet = {.intatr = TA_HLNG, .inthdr = (FP)interrupt};
  return tk_def_int((UINT)self->interrupt_number, &packet) == E_OK;
}
static Isr New(int interrupt_number, InterruptDelegate interrupt) {
  if (!Validate(interrupt_number, interrupt)) return NULL;
  Isr self = (Isr)heap->New(sizeof(IsrStruct));
  if (!self) return self;
  self->interrupt_number = interrupt_number;
  if (!Register(self, interrupt)) heap->Delete((void**)&self);
  return self;
}
static void InterruptDummy(int unused) {}
inline static void Unregister(Isr self) { Register(self, InterruptDummy); }
static void Delete(Isr* self) {
  if (!self || !(*self)) return;
  DisableInt((UINT)(*self)->interrupt_number);
  Unregister(*self);
  heap->Delete((void**)self);
}
static void Enable(Isr self, int level) {
  if (self && level >= 0) EnableInt((UINT)self->interrupt_number, level);
}
static void Disable(Isr self) {
  if (self) DisableInt((UINT)self->interrupt_number);
}
static const IsrMethodStruct kTheMethod = {
    .New = New, .Delete = Delete, .Enable = Enable, .Disable = Disable,
};
const IsrMethod isr = &kTheMethod;
