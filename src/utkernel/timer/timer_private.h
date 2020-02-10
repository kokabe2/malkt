// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_UTKERNEL_TIMER_TIMER_PRIVATE_H_
#define SRC_UTKERNEL_TIMER_TIMER_PRIVATE_H_

#include <stdbool.h>

#include "timer.h"

typedef struct TimerStruct {
  int id;
  TimerDelegate timer;
  TimerAbstractMethod impl;
} TimerStruct;

typedef struct {
  bool (*CreateTimer)(Timer self, int delay_in_milliseconds,
                      int period_in_milliseconds, void (*handler)(void* exinf));
} TimerPrivateMethodStruct;
typedef const TimerPrivateMethodStruct* TimerPrivateMethod;

extern const TimerPrivateMethod _timer;

#endif  // SRC_UTKERNEL_TIMER_TIMER_PRIVATE_H_
