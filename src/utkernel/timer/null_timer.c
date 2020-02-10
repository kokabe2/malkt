// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "null_timer.h"

#include "timer_private.h"

static void Resume(Timer unused) {}
static const TimerAbstractMethodStruct kTheMethod = {
    .Resume = Resume,
};
static const TimerStruct kTheInstance = {
    .id = -1, .impl = &kTheMethod,
};  // Use const for .rodata section.
const Timer kNullTimer = (Timer)&kTheInstance;
