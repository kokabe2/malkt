// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "task_spy.h"

#include <stddef.h>

#include "task.h"

typedef struct TaskStruct {
  int dummy;  //
} TaskStruct;

static TaskStruct task_dummy;
static int new_called_count;
static int delete_called_count;
static int run_called_count;
static int suspend_called_count;
static int resume_called_count;
static int delay_called_count;

static void Reset(void) {
  new_called_count = 0;
  delete_called_count = 0;
  run_called_count = 0;
  suspend_called_count = 0;
  resume_called_count = 0;
  delay_called_count = 0;
}
static int NewCalledCount(void) { return new_called_count; }
static int DeleteCalledCount(void) { return delete_called_count; }
static int RunCalledCount(void) { return run_called_count; }
static int SuspendCalledCount(void) { return suspend_called_count; }
static int ResumeCalledCount(void) { return resume_called_count; }
static int DelayCalledCount(void) { return delay_called_count; }
static const TaskSpyMethodStruct kSpyMethod = {
    .Reset = Reset,
    .NewCalledCount = NewCalledCount,
    .DeleteCalledCount = DeleteCalledCount,
    .RunCalledCount = RunCalledCount,
    .SuspendCalledCount = SuspendCalledCount,
    .ResumeCalledCount = ResumeCalledCount,
    .DelayCalledCount = DelayCalledCount,
};
const TaskSpyMethod taskSpy = &kSpyMethod;

static Task New(ActionDelegate action, int priority, int stack_size) {
  ++new_called_count;
  return &task_dummy;
}
static void Delete(Task* self) {
  ++delete_called_count;
  if (self) *self = NULL;
}
static void Run(Task self) { ++run_called_count; }
static void Suspend(Task self) { ++suspend_called_count; }
static void Resume(Task self) { ++resume_called_count; }
static void Delay(int time_in_milliseconds) { ++delay_called_count; }
static const TaskMethodStruct kTheMethod = {
    .New = New, .Delete = Delete, .Run = Run, .Suspend = Suspend, .Resume = Resume, .Delay = Delay,
};
const TaskMethod task = &kTheMethod;
