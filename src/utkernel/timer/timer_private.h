// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_UTKERNEL_TIMER_TIMER_PRIVATE_H_
#define SRC_UTKERNEL_TIMER_TIMER_PRIVATE_H_

#include "timer.h"

typedef struct TimerStruct {
  int id;
  TimerDelegate timer;
} TimerStruct;

#endif  // SRC_UTKERNEL_TIMER_TIMER_PRIVATE_H_
