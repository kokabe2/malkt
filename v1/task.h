// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_TASK_H_
#define V1_TASK_H_

typedef struct TaskInterfaceStruct* Task;
typedef struct TaskInterfaceStruct {
  void (*Delete)(Task* self);
  void (*Run)(Task self);
  void (*Suspend)(Task self);
  void (*Resume)(Task self);
} TaskInterfaceStruct;
typedef const TaskInterfaceStruct* TaskInterface;

extern const TaskInterface task;

#endif  // V1_TASK_H_
