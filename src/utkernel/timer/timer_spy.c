// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "timer_spy.h"

#include <stddef.h>

#include "timer.h"

typedef struct TimerStruct {
  int dummy;  //
} TimerStruct;

static int delete_called_count;
static int pause_callde_count;
static int resume_called_count;

static void Reset(void) {
  delete_called_count = 0;
  pause_callde_count = 0;
  resume_called_count = 0;
}

static int DeleteCalledCount(void) { return delete_called_count; }

static int PauseCalledCount(void) { return pause_callde_count; }

static int ResumeCalledCount(void) { return resume_called_count; }

static const TimerSpyMethodStruct kSpyMethod = {
    .Reset = Reset,
    .DeleteCalledCount = DeleteCalledCount,
    .PauseCalledCount = PauseCalledCount,
    .ResumeCalledCount = ResumeCalledCount,
};

const TimerSpyMethod timerSpy = &kSpyMethod;

static void Delete(Timer* self) {
  ++delete_called_count;
  if (self) *self = NULL;
}

static void Pause(Timer self) { ++pause_callde_count; }

static void Resume(Timer self) { ++resume_called_count; }

static const TimerAbstractMethodStruct kTheMethod = {
    .Delete = Delete, .Pause = Pause, .Resume = Resume,
};

const TimerAbstractMethod timer = &kTheMethod;
