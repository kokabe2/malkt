// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_UTKERNEL_TIMER_TIMER_HANDLER_SPY_H_
#define SRC_UTKERNEL_TIMER_TIMER_HANDLER_SPY_H_

#include <stdbool.h>

#include "timer.h"

typedef struct {
  TimerDelegate (*Get)(void);
  void (*Reset)(void);
  bool (*WasRun)(void);
} TimerHandlerSpyMethodStruct;
typedef const TimerHandlerSpyMethodStruct* TimerHandlerSpyMethod;

extern const TimerHandlerSpyMethod timerHandlerSpy;

#endif  // SRC_UTKERNEL_TIMER_TIMER_HANDLER_SPY_H_
