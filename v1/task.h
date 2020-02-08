// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_TASK_H_
#define V1_TASK_H_

enum {
  kLowestTaskPriority = 1,
  kHighestTaskPriority = 8,
  kMaxTaskStackSize = 4 * 1024,
};

typedef struct TaskStruct* Task;
typedef void (*ActionDelegate)(void);
typedef struct {
  Task (*New)(ActionDelegate action, int priority, int stack_size);
  void (*Delete)(Task* self);
  void (*Run)(Task self);
  void (*Suspend)(Task self);
  void (*Resume)(Task self);
  void (*Delay)(int time_in_milliseconds);
} TaskMethodStruct;
typedef const TaskMethodStruct* TaskMethod;

extern const TaskMethod task;

#endif  // V1_TASK_H_
