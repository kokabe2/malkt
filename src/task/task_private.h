// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_TASK_TASK_PRIVATE_H_
#define SRC_TASK_TASK_PRIVATE_H_

#include "task.h"

typedef struct TaskStruct {
  TaskInterface impl;
  int id;
} TaskStruct;

#endif  // SRC_TASK_TASK_PRIVATE_H_
