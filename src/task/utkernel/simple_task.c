// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "simple_task.h"

#include <stdbool.h>
#include <stddef.h>

#include "bleu/v1/heap.h"
#include "utkernel/utkernel.h"

enum {
  kPriorityOffset = 5,
};

typedef int (*ResumeDelegate)(int id);
typedef struct {
  TaskInterfaceStruct impl;
  int id;
  ActionDelegate Action;
  ResumeDelegate Resume;
} SimpleTaskStruct, *SimpleTask;

inline static bool IsRunning(SimpleTask self) { return self->id == tk_get_tid(); }

inline static void KillMyself(SimpleTask* self) {
  heap->Delete((void**)self);
  tk_exd_tsk();
}

inline static void KillOther(SimpleTask* self) {
  tk_ter_tsk((*self)->id);
  tk_del_tsk((*self)->id);
  heap->Delete((void**)self);
}

static void Delete(Task* base) {
  SimpleTask* self = (SimpleTask*)base;
  if (IsRunning(*self))
    KillMyself(self);
  else
    KillOther(self);
}

static void Run(Task self) { tk_sta_tsk(((SimpleTask)self)->id, 0); }

inline static bool IsSuspended(SimpleTask self) { return self->Resume != NULL; }

inline static void Sleep(SimpleTask self) {
  self->Resume = tk_wup_tsk;
  tk_slp_tsk(TMO_FEVR);
}

inline static void SuspendOther(SimpleTask self) {
  self->Resume = tk_rsm_tsk;
  tk_sus_tsk(self->id);
  tk_rel_wai(self->id);  // In case of task state is Blocked.
}

static void Suspend(Task base) {
  SimpleTask self = (SimpleTask)base;
  if (IsSuspended(self)) return;

  if (IsRunning(self))
    Sleep(self);
  else
    SuspendOther(self);
}

static void Resume(Task base) {
  SimpleTask self = (SimpleTask)base;
  if (IsSuspended(self)) {
    ResumeDelegate Resume = self->Resume;
    self->Resume = NULL;
    Resume(self->id);
  }
}

static const TaskInterfaceStruct kTheInterface = {
    .Delete = Delete, .Run = Run, .Suspend = Suspend, .Resume = Resume,
};

static void TaskEntry(int unused, void* exinf) {
  SimpleTask self = (SimpleTask)exinf;
  self->Action();
  tk_ext_tsk();
}

inline static int LimitPriority(int task_priority) {
  if (task_priority > kHighestTaskPriority)
    return kHighestTaskPriority;
  else if (task_priority < kLowestTaskPriority)
    return kLowestTaskPriority;
  else
    return task_priority;
}

inline static int ReversePriority(int task_priority) { return kHighestTaskPriority - task_priority + kPriorityOffset; }

inline static int AdjustPriority(int task_priority) {
  int limited = LimitPriority(task_priority);
  return ReversePriority(limited);
}

inline static int LimitStackSize(int stack_size) {
  return stack_size > kMaxTaskStackSize ? kMaxTaskStackSize : stack_size;
}

inline static void CreateTask(SimpleTask self, int task_priority, int stack_size) {
  T_CTSK packet = {.exinf = (void*)self,
                   .tskatr = (TA_HLNG | TA_RNG0),
                   .task = (FP)TaskEntry,
                   .itskpri = (PRI)AdjustPriority(task_priority),
                   .stksz = (SZ)LimitStackSize(stack_size)};
  self->id = tk_cre_tsk(&packet);
}

static Task New(ActionDelegate Action, int task_priority, int stack_size) {
  SimpleTask self = (SimpleTask)heap->New(sizeof(SimpleTaskStruct));
  self->impl = kTheInterface;
  self->Action = Action;
  CreateTask(self, task_priority, stack_size);
  return (Task)self;
}

static const SimpleTaskMethodStruct kTheMethod = {
    .New = New,
};

const SimpleTaskMethod simpleTask = &kTheMethod;
