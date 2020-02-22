// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_TIMER_SPY_H_
#define V1_TIMER_SPY_H_

typedef struct {
  void (*Reset)(void);
  int (*DeleteCalledCount)(void);
  int (*PauseCalledCount)(void);
  int (*ResumeCalledCount)(void);
} TimerSpyMethodStruct;
typedef const TimerSpyMethodStruct* TimerSpyMethod;

extern const TimerSpyMethod timerSpy;

#endif  // V1_TIMER_SPY_H_
