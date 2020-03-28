// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "basic_isr.h"

#include "../isr_private.h"
#include "bleu/v1/heap.h"
#include "utkernel/utkernel.h"

inline static void Register(Isr self, InterruptDelegate delegate) {
  T_DINT packet = {.intatr = TA_HLNG, .inthdr = (FP)delegate};
  tk_def_int((UINT)self->number, &packet);
}

static void DummyInterrupt(int unused) {}

inline static void Unregister(Isr self) { Register(self, DummyInterrupt); }

static void Delete(Isr* self) {
  DisableInt((UINT)(*self)->number);
  Unregister(*self);
  heap->Delete((void**)self);
}

static void Enable(Isr self) { EnableInt((UINT)self->number, self->level); }

static void Disable(Isr self) { DisableInt((UINT)self->number); }

static const IsrInterfaceStruct kTheInterface = {
    .Delete = Delete,
    .Enable = Enable,
    .Disable = Disable,
};

static Isr New(int interrupt_number, int interrupt_level, InterruptDelegate delegate) {
  Isr self = (Isr)heap->New(sizeof(IsrStruct));
  self->impl = &kTheInterface;
  self->number = interrupt_number;
  self->level = interrupt_level;
  Register(self, delegate);
  return self;
}

static const BasicIsrMethodStruct kTheMethod = {
    .New = New,
};

const BasicIsrMethod basicIsr = &kTheMethod;
