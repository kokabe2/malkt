// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_UTKERNEL_TIMER_TIMER_SPY_H_
#define SRC_UTKERNEL_TIMER_TIMER_SPY_H_

#include <stdbool.h>

#include "timer.h"

typedef struct {
  TimerDelegate (*Get)(void);
  void (*Reset)(void);
  bool (*WasRun)(void);
} TimerSpyMethodStruct;
typedef const TimerSpyMethodStruct* TimerSpyMethod;

extern const TimerSpyMethod timerSpy;

#endif  // SRC_UTKERNEL_TIMER_TIMER_SPY_H_
