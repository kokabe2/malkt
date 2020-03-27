// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_TIMER_UTKERNEL_TIMER_PROTECTED_H_
#define SRC_TIMER_UTKERNEL_TIMER_PROTECTED_H_

#include "timer.h"

typedef struct TimerStruct {
  int id;
  TimerDelegate timer;
  TimerAbstractMethod impl;
} TimerStruct;

typedef struct {
  void (*CreateTimer)(Timer self, int delay_in_milliseconds, int period_in_milliseconds, void (*handler)(void* exinf));
} TimerProtectedMethodStruct;
typedef const TimerProtectedMethodStruct* TimerProtectedMethod;

extern const TimerProtectedMethod _timer;

#endif  // SRC_TIMER_UTKERNEL_TIMER_PROTECTED_H_
