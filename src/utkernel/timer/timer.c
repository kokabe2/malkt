// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "timer.h"

#include "bleu/v1/heap.h"
#include "null_timer.h"
#include "timer_private.h"
#include "utkernel/utkernel.h"

static void Delete(Timer* self) {
  if (!self || !*self || *self == kNullTimer) return;
  tk_del_cyc((*self)->id);
  heap->Delete((void**)self);
}
static void Pause(Timer self) {
  if (self) tk_stp_cyc(self->id);
}
static void Resume(Timer self) {
  if (self) self->impl->Resume(self);
}
static const TimerAbstractMethodStruct kTheMethod = {
    .Delete = Delete, .Pause = Pause, .Resume = Resume,
};
const TimerAbstractMethod timer = &kTheMethod;
