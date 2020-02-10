// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "timer.h"

#include "bleu/v1/heap.h"
#include "timer_private.h"
#include "utkernel/utkernel.h"

static void Delete(Timer* self) {
  if (!self || !*self) return;
  tk_del_cyc((*self)->id);
  heap->Delete((void**)self);
}
static void Pause(Timer self) {
  if (self) tk_stp_cyc(self->id);
}
static void Resume(Timer self) {
  if (self) tk_sta_cyc(self->id);
}
static const TimerAbstractMethodStruct kTheMethod = {
    .Delete = Delete, .Pause = Pause, .Resume = Resume,
};
const TimerAbstractMethod timer = &kTheMethod;
