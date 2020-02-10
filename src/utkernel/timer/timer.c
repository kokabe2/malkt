// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "timer.h"

#include <stdbool.h>
#include <stddef.h>

#include "bleu/v1/heap.h"
#include "timer_private.h"
#include "utkernel/utkernel.h"

inline static bool Validate(TimerDelegate timer,
                            int base_time_in_milliseconds) {
  return timer && base_time_in_milliseconds > 0;
}
inline static bool CreateTimer(Timer self, int base_time_in_milliseconds,
                               void (*handler)(void* exinf)) {
  T_CCYC packet = {.exinf = self,
                   .cycatr = TA_HLNG | TA_STA | TA_PHS,
                   .cychdr = handler,
                   .cyctim = (RELTIM)base_time_in_milliseconds,
                   .cycphs = (RELTIM)base_time_in_milliseconds};
  return (self->id = tk_cre_cyc(&packet)) >= 0;
}
static Timer New(TimerDelegate timer, int base_time_in_milliseconds,
                 void (*handler)(void* exinf), TimerAbstractMethod impl) {
  if (!Validate(timer, base_time_in_milliseconds)) return NULL;
  Timer self = (Timer)heap->New(sizeof(TimerStruct));
  if (!self) return self;
  self->timer = timer;
  self->impl = impl;
  if (!CreateTimer(self, base_time_in_milliseconds, handler))
    heap->Delete((void**)&self);
  return self;
}
static const TimerPrivateMethodStruct kPrivateMethod = {
    .New = New,
};
const TimerPrivateMethod _timer = &kPrivateMethod;

static void Delete(Timer* self) {
  if (!self || !*self) return;
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
