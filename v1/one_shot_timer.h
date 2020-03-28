// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_ONE_SHOT_TIMER_H_
#define V1_ONE_SHOT_TIMER_H_

#include <stdbool.h>

#include "timer.h"

typedef void (*OneShotTimerDelegate)(void);
typedef struct {
  Timer (*New)(OneShotTimerDelegate timer, int milliseconds);
  bool (*IsDone)(Timer self);
} OneShotTimerMethodStruct;
typedef const OneShotTimerMethodStruct* OneShotTimerMethod;

extern const OneShotTimerMethod oneShotTimer;

#endif  // V1_ONE_SHOT_TIMER_H_
