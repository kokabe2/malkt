// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "one_shot_timer.h"

#include "bleu/v1/heap.h"
#include "timer_protected.h"
#include "utkernel/utkernel.h"

typedef struct {
  TimerStruct base;
  bool is_done;
} OneShotTimerStruct, *OneShotTimer;

inline static OneShotTimer Downcast(Timer self) { return (OneShotTimer)self; }

inline static bool IsDone(Timer self) { return Downcast(self)->is_done; }

static void Resume(Timer self) {
  if (!IsDone(self)) tk_sta_cyc(self->id);
}

static const TimerAbstractMethodStruct kConcreteMethod = {
    .Resume = Resume,
};

inline static void Done(Timer self) { Downcast(self)->is_done = true; }

static void TimerEntry(void* exinf) {
  Timer self = (Timer)exinf;
  if (!IsDone(self)) {
    self->timer();
    Done(self);
  }
}

static Timer New(TimerDelegate timer, int milliseconds) {
  Timer self = (Timer)heap->New(sizeof(OneShotTimerStruct));
  self->timer = timer;
  self->impl = &kConcreteMethod;
  _timer->CreateTimer(self, milliseconds, ~0, TimerEntry);
  return self;
}

static const OneShotTimerMethodStruct kTheMethod = {
    .New = New, .IsDone = IsDone,
};

const OneShotTimerMethod oneShotTimer = &kTheMethod;
