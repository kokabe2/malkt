// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_TASK_SPY_H_
#define V1_TASK_SPY_H_

typedef struct {
  void (*Reset)(void);
  int (*NewCalledCount)(void);
  int (*DeleteCalledCount)(void);
  int (*RunCalledCount)(void);
  int (*SuspendCalledCount)(void);
  int (*ResumeCalledCount)(void);
  int (*DelayCalledCount)(void);
} TaskSpyMethodStruct;
typedef const TaskSpyMethodStruct* TaskSpyMethod;

extern const TaskSpyMethod taskSpy;

#endif  // V1_TASK_SPY_H_
