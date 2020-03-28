// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_TIMER_H_
#define V1_TIMER_H_

typedef struct TimerStruct* Timer;
typedef struct {
  void (*Delete)(Timer* self);
  void (*Pause)(Timer self);
  void (*Resume)(Timer self);
} TimerInterfaceStruct;
typedef const TimerInterfaceStruct* TimerInterface;

extern const TimerInterface timer;

#endif  // V1_TIMER_H_
