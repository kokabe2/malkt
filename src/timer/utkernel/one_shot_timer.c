// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "one_shot_timer.h"

#include "bleu/v1/heap.h"
#include "utkernel/utkernel.h"

typedef struct {
  TimerInterfaceStruct impl;
  int id;
  OneShotTimerDelegate Timer;
  bool done;
} OneShotTimerStruct, *OneShotTimer;

static void Delete(Timer* self) {
  tk_del_cyc(((OneShotTimer)(*self))->id);
  heap->Delete((void**)self);
}

static void Pause(Timer self) { tk_stp_cyc(((OneShotTimer)self)->id); }

inline static bool IsDone(Timer self) { return ((OneShotTimer)self)->done; }

static void Resume(Timer self) {
  if (!IsDone(self)) tk_sta_cyc(((OneShotTimer)self)->id);
}

static const TimerInterfaceStruct kTheInterface = {
    .Delete = Delete, .Pause = Pause, .Resume = Resume,
};

static void TimerEntry(void* exinf) {
  OneShotTimer self = (OneShotTimer)exinf;
  if (!IsDone((Timer)self)) {
    self->Timer();
    self->done = true;
  }
}

static void CreateTimer(OneShotTimer self, int milliseconds) {
  T_CCYC packet = {.exinf = self,
                   .cycatr = TA_HLNG | TA_STA | TA_PHS,
                   .cychdr = (FP)TimerEntry,
                   .cyctim = (RELTIM)~0,
                   .cycphs = (RELTIM)milliseconds};
  self->id = tk_cre_cyc(&packet);
}

static Timer New(OneShotTimerDelegate timer, int milliseconds) {
  OneShotTimer self = (OneShotTimer)heap->New(sizeof(OneShotTimerStruct));
  self->impl = kTheInterface;
  self->Timer = timer;
  CreateTimer(self, milliseconds);
  return (Timer)self;
}

static const OneShotTimerMethodStruct kTheMethod = {
    .New = New, .IsDone = IsDone,
};

const OneShotTimerMethod oneShotTimer = &kTheMethod;
