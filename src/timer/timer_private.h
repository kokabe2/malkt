// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_TIMER_TIMER_PRIVATE_H_
#define SRC_TIMER_TIMER_PRIVATE_H_

#include "timer.h"

typedef struct TimerStruct {
  TimerInterface impl;
  int id;
} TimerStruct;

#endif  // SRC_TIMER_TIMER_PRIVATE_H_
