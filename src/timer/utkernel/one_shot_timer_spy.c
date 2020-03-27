// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "one_shot_timer_spy.h"

#include <string.h>

#include "one_shot_timer.h"

typedef struct OneShotTimerStruct {
  bool is_done;  //
} OneShotTimerStruct, *OneShotTimer;

static int new_called_count;
static OneShotTimerStruct the_instances[32];  // Maybe enough.

static void Reset(void) {
  new_called_count = 0;
  memset(the_instances, 0, sizeof(the_instances));
}

static int NewCalledCount(void) { return new_called_count; }

static void Run(int id) { the_instances[id].is_done = true; }

static const OneShotTimerSpyMethodStruct kSpyMethod = {
    .Reset = Reset, .NewCalledCount = NewCalledCount, .Run = Run,
};

const OneShotTimerSpyMethod oneShotTimerSpy = &kSpyMethod;

static Timer New(TimerDelegate timer, int delay_in_milliseconds) { return (Timer)&the_instances[new_called_count++]; }

static bool IsDone(Timer self) { return ((OneShotTimer)self)->is_done; }

static const OneShotTimerMethodStruct kTheMethod = {
    .New = New, .IsDone = IsDone,
};

const OneShotTimerMethod oneShotTimer = &kTheMethod;
