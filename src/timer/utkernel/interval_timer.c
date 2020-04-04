// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "interval_timer.h"

#include "bleu/v1/heap.h"
#include "utkernel/utkernel.h"

typedef struct {
  TimerInterfaceStruct impl;
  int id;
  IntervalTimerDelegate Timer;
} IntervalTimerStruct, *IntervalTimer;

static void Delete(Timer* self) {
  tk_del_cyc(((IntervalTimer)(*self))->id);
  heap->Delete((void**)self);
}

static void Pause(Timer self) { tk_stp_cyc(((IntervalTimer)self)->id); }

static void Resume(Timer self) { tk_sta_cyc(((IntervalTimer)self)->id); }

static const TimerInterfaceStruct kTheInterface = {
    .Delete = Delete, .Pause = Pause, .Resume = Resume,
};

static void TimerEntry(void* exinf) {
  IntervalTimer self = (IntervalTimer)exinf;
  self->Timer();
}

static void CreateTimer(IntervalTimer self, int milliseconds) {
  T_CCYC packet = {.exinf = self,
                   .cycatr = TA_HLNG | TA_STA | TA_PHS,
                   .cychdr = (FP)TimerEntry,
                   .cyctim = (RELTIM)milliseconds,
                   .cycphs = (RELTIM)milliseconds};
  self->id = tk_cre_cyc(&packet);
}

static Timer New(IntervalTimerDelegate timer, int milliseconds) {
  IntervalTimer self = (IntervalTimer)heap->New(sizeof(IntervalTimerStruct));
  self->impl = kTheInterface;
  self->Timer = timer;
  CreateTimer(self, milliseconds);
  return (Timer)self;
}

static const IntervalTimerMethodStruct kTheMethod = {
    .New = New,
};

const IntervalTimerMethod intervalTimer = &kTheMethod;
