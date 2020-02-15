// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "one_shot_timer.h"

#include <stddef.h>

#include "bleu/v1/heap.h"
#include "timer_private.h"
#include "utkernel/utkernel.h"

typedef struct {
  TimerStruct base;
  bool is_done;
} OneShotTimerStruct, *OneShotTimer;

inline static OneShotTimer Downcast(Timer self) { return (OneShotTimer)self; }
static void Resume(Timer self) {
  if (!Downcast(self)->is_done) tk_sta_cyc(self->id);
}
static const TimerAbstractMethodStruct kConcreteMethod = {
    .Resume = Resume,
};

inline static bool Validate(TimerDelegate timer, int delay_in_milliseconds) {
  return timer && delay_in_milliseconds > 0;
}
static void TimerEntry(void* exinf) {
  OneShotTimer self = (OneShotTimer)exinf;
  self->base.timer();
  self->is_done = true;
}
static Timer New(TimerDelegate timer, int delay_in_milliseconds) {
  OneShotTimer self = Validate(timer, delay_in_milliseconds)
                          ? (OneShotTimer)heap->New(sizeof(OneShotTimerStruct))
                          : NULL;
  if (!self) return (Timer)self;
  self->base.timer = timer;
  self->base.impl = &kConcreteMethod;
  if (!_timer->CreateTimer((Timer)self, delay_in_milliseconds, ~0, TimerEntry))
    heap->Delete((void**)&self);
  return (Timer)self;
}
static bool IsDone(Timer self) {
  return self ? Downcast(self)->is_done : false;
}
static const OneShotTimerMethodStruct kTheMethod = {
    .New = New, .IsDone = IsDone,
};
const OneShotTimerMethod oneShotTimer = &kTheMethod;
