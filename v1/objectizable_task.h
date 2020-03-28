// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_OBJECTIZABLE_TASK_H_
#define V1_OBJECTIZABLE_TASK_H_

#include "task.h"

enum {
  kLowestTaskPriority = 1,
  kHighestTaskPriority = 8,
  kMaxTaskStackSize = 4 * 1024,
};

typedef void (*ActionDelegate)(void* object);
typedef struct {
  Task (*New)(ActionDelegate delegate, int task_priority, int stack_size, void* object);
} ObjectizableTaskMethodStruct;
typedef const ObjectizableTaskMethodStruct* ObjectizableTaskMethod;

extern const ObjectizableTaskMethod objectizableTask;

#endif  // V1_OBJECTIZABLE_TASK_H_
