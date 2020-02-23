// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_ONE_SHOT_TIMER_SPY_H_
#define V1_ONE_SHOT_TIMER_SPY_H_

typedef struct {
  void (*Reset)(void);
  int (*NewCalledCount)(void);
  void (*Run)(int id);
} OneShotTimerSpyMethodStruct;
typedef const OneShotTimerSpyMethodStruct* OneShotTimerSpyMethod;

extern const OneShotTimerSpyMethod oneShotTimerSpy;

#endif  // V1_ONE_SHOT_TIMER_SPY_H_
