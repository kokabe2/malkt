// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "timer.h"

#include "bleu/v1/heap.h"
#include "timer_protected.h"
#include "utkernel/utkernel.h"

static bool CreateTimer(Timer self, int delay_in_milliseconds, int period_in_milliseconds,
                        void (*handler)(void* exinf)) {
  T_CCYC packet = {.exinf = self,
                   .cycatr = TA_HLNG | TA_STA | TA_PHS,
                   .cychdr = (FP)handler,
                   .cyctim = (RELTIM)period_in_milliseconds,
                   .cycphs = (RELTIM)delay_in_milliseconds};
  return (self->id = tk_cre_cyc(&packet)) >= 0;
}

static const TimerProtectedMethodStruct kProtectedMethod = {
    .CreateTimer = CreateTimer,
};

const TimerProtectedMethod _timer = &kProtectedMethod;

static void Delete(Timer* self) {
  tk_del_cyc((*self)->id);
  heap->Delete((void**)self);
}

static void Pause(Timer self) { tk_stp_cyc(self->id); }

static void Resume(Timer self) { self->impl->Resume(self); }

static const TimerAbstractMethodStruct kTheMethod = {
    .Delete = Delete, .Pause = Pause, .Resume = Resume,
};

const TimerAbstractMethod timer = &kTheMethod;
