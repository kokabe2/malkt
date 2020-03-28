// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_SIMPLE_TASK_H_
#define V1_SIMPLE_TASK_H_

#include "task.h"

enum {
  kLowestTaskPriority = 1,
  kHighestTaskPriority = 8,
  kMaxTaskStackSize = 4 * 1024,
};

typedef void (*ActionDelegate)(void);
typedef struct {
  Task (*New)(ActionDelegate delegate, int task_priority, int stack_size);  //
} SimpleTaskMethodStruct;
typedef const SimpleTaskMethodStruct* SimpleTaskMethod;

extern const SimpleTaskMethod simpleTask;

#endif  // V1_SIMPLE_TASK_H_
