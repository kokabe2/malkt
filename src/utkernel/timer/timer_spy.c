// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "timer_spy.h"

static bool was_run;

static void TimerFunction(void) { was_run = true; }
static TimerDelegate Get(void) { return TimerFunction; }
static void Reset(void) { was_run = false; }
static bool WasRun(void) { return was_run; }
static const TimerSpyMethodStruct kTheMethod = {
    .Get = Get, .Reset = Reset, .WasRun = WasRun,
};
const TimerSpyMethod timerSpy = &kTheMethod;
