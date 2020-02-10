// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "interval_timer.h"

#include "timer_private.h"
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
  return _timer->New(timer, period_in_milliseconds, TimerEntry,
                     &kConcreteMethod);
}
static const IntervalTimerMethodStruct kTheMethod = {
    .New = New,
};
const IntervalTimerMethod intervalTimer = &kTheMethod;
