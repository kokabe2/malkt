﻿// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "interval_timer.h"

#include <stdbool.h>
#include <stddef.h>

#include "bleu/v1/heap.h"
#include "timer_private.h"
#include "utkernel/utkernel.h"

inline static bool Validate(TimerDelegate timer, int period_in_milliseconds) {
  return timer && period_in_milliseconds > 0;
}
static void TimerEntry(void* exinf) {
  Timer self = (Timer)exinf;
  self->timer();
}
inline static bool CreateTimer(Timer self, int period_in_milliseconds) {
  T_CCYC packet = {.exinf = self,
                   .cycatr = TA_HLNG | TA_STA | TA_PHS,
                   .cychdr = TimerEntry,
                   .cyctim = period_in_milliseconds,
                   .cycphs = period_in_milliseconds};
  return (self->id = tk_cre_cyc(&packet)) >= 0;
}
static Timer New(TimerDelegate timer, int period_in_milliseconds) {
  if (!Validate(timer, period_in_milliseconds)) return NULL;
  Timer self = (Timer)heap->New(sizeof(TimerStruct));
  if (!self) return self;
  self->timer = timer;
  if (!CreateTimer(self, period_in_milliseconds)) heap->Delete((void**)&self);
  return self;
}
static const IntervalTimerMethodStruct kTheMethod = {
    .New = New,
};
const IntervalTimerMethod intervalTimer = &kTheMethod;
