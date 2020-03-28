// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_INTERVAL_TIMER_H_
#define V1_INTERVAL_TIMER_H_

#include "timer.h"

typedef void (*IntervalTimerDelegate)(void);
typedef struct {
  Timer (*New)(IntervalTimerDelegate timer, int milliseconds);  //
} IntervalTimerMethodStruct;
typedef const IntervalTimerMethodStruct* IntervalTimerMethod;

extern const IntervalTimerMethod intervalTimer;

#endif  // V1_INTERVAL_TIMER_H_
