// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_UTKERNEL_TIMER_TIMER_PRIVATE_H_
#define SRC_UTKERNEL_TIMER_TIMER_PRIVATE_H_

#include "timer.h"

typedef struct TimerStruct {
  int id;
  TimerDelegate timer;
  TimerAbstractMethod impl;
} TimerStruct;

typedef struct {
  Timer (*New)(TimerDelegate timer, int base_time_in_milliseconds,
               void (*handler)(void* exinf), TimerAbstractMethod impl);
} TimerPrivateMethodStruct;
typedef const TimerPrivateMethodStruct* TimerPrivateMethod;

extern const TimerPrivateMethod _timer;

#endif  // SRC_UTKERNEL_TIMER_TIMER_PRIVATE_H_
