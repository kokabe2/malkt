// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "interval_timer.h"

#include "bleu/v1/heap.h"
#include "timer_protected.h"
#include "utkernel/utkernel.h"

static void Resume(Timer self) { tk_sta_cyc(self->id); }

static const TimerAbstractMethodStruct kConcreteMethod = {
    .Resume = Resume,
};

static void TimerEntry(void* exinf) {
  Timer self = (Timer)exinf;
  self->timer();
}

static Timer New(TimerDelegate timer, int period_in_milliseconds) {
  Timer self = (Timer)heap->New(sizeof(TimerStruct));
  self->timer = timer;
  self->impl = &kConcreteMethod;
  _timer->CreateTimer(self, period_in_milliseconds, period_in_milliseconds, TimerEntry);
  return self;
}

static const IntervalTimerMethodStruct kTheMethod = {
    .New = New,
};

const IntervalTimerMethod intervalTimer = &kTheMethod;
