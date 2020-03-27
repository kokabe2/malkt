// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "timer_handler_spy.h"

static bool was_run;

static void TimerHandler(void) { was_run = true; }

static TimerDelegate Get(void) { return TimerHandler; }

static void Reset(void) { was_run = false; }

static bool WasRun(void) { return was_run; }

static const TimerHandlerSpyMethodStruct kTheMethod = {
    .Get = Get, .Reset = Reset, .WasRun = WasRun,
};

const TimerHandlerSpyMethod timerHandlerSpy = &kTheMethod;
