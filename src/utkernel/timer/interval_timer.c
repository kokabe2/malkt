// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "interval_timer.h"

#include <stdbool.h>
#include <stddef.h>

#include "bleu/v1/heap.h"
#include "timer_private.h"
#include "utkernel/utkernel.h"

static void Resume(Timer self) { tk_sta_cyc(self->id); }
static const TimerAbstractMethodStruct kConcreteMethod = {
    .Resume = Resume,
};

inline static bool Validate(TimerDelegate timer, int period_in_milliseconds) {
  return timer && period_in_milliseconds > 0;
}
static void TimerEntry(void* exinf) {
  Timer self = (Timer)exinf;
  self->timer();
}
static Timer New(TimerDelegate timer, int period_in_milliseconds) {
  if (!Validate(timer, period_in_milliseconds)) return NULL;
  Timer self = (Timer)heap->New(sizeof(TimerStruct));
  if (!self) return self;
  self->timer = timer;
  self->impl = &kConcreteMethod;
  if (!_timer->CreateTimer(self, period_in_milliseconds, period_in_milliseconds,
                           TimerEntry))
    heap->Delete((void**)&self);
  return self;
}
static const IntervalTimerMethodStruct kTheMethod = {
    .New = New,
};
const IntervalTimerMethod intervalTimer = &kTheMethod;
