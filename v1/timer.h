// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_TIMER_H_
#define V1_TIMER_H_

typedef void (*TimerDelegate)(void);
typedef struct TimerStruct* Timer;
typedef struct {
  void (*Delete)(Timer* self);
  void (*Pause)(Timer self);
  void (*Resume)(Timer self);
} TimerAbstractMethodStruct;
typedef const TimerAbstractMethodStruct* TimerAbstractMethod;

extern const TimerAbstractMethod timer;

#endif  // V1_TIMER_H_
