// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "task.h"

#include <stdbool.h>
#include <stddef.h>

#include "bleu/v1/heap.h"
#include "utkernel/utkernel.h"

enum {
  kPriorityOffset = 5,
};

typedef int (*ResumeDelegate)(int id);
typedef struct TaskStruct {
  int id;
  ActionDelegate action;
  ResumeDelegate resume;
} TaskStruct;

static void TaskEntry(int unused, void* exinf) {
  Task self = (Task)exinf;
  self->action();
  tk_ext_tsk();
}

inline static int LimitPriority(int priority) {
  if (priority > kHighestTaskPriority)
    return kHighestTaskPriority;
  else if (priority < kLowestTaskPriority)
    return kLowestTaskPriority;
  else
    return priority;
}

inline static int ReversePriority(int priority) { return kHighestTaskPriority - priority + kPriorityOffset; }

inline static int AdjustPriority(int priority) {
  int limited = LimitPriority(priority);
  return ReversePriority(limited);
}

inline static int LimitStackSize(int stack_size) {
  return stack_size > kMaxTaskStackSize ? kMaxTaskStackSize : stack_size;
}

inline static void CreateTask(Task self, int priority, int stack_size) {
  T_CTSK packet = {.exinf = (void*)self,
                   .tskatr = (TA_HLNG | TA_RNG0),
                   .task = (FP)TaskEntry,
                   .itskpri = (PRI)AdjustPriority(priority),
                   .stksz = (SZ)LimitStackSize(stack_size)};
  self->id = tk_cre_tsk(&packet);
}

static Task New(ActionDelegate action, int priority, int stack_size) {
  Task self = (Task)heap->New(sizeof(TaskStruct));
  self->action = action;
  CreateTask(self, priority, stack_size);
  return self;
}

inline static bool IsMyself(Task self) { return self->id == tk_get_tid(); }

inline static void KillMyself(Task* self) {
  heap->Delete((void**)self);
  tk_exd_tsk();
}

inline static void KillOther(Task* self) {
  tk_ter_tsk((*self)->id);
  tk_del_tsk((*self)->id);
  heap->Delete((void**)self);
}

static void Delete(Task* self) {
  if (IsMyself(*self))
    KillMyself(self);
  else
    KillOther(self);
}

static void Run(Task self) { tk_sta_tsk(self->id, 0); }

inline static bool IsSuspended(Task self) { return self->resume != NULL; }

inline static void Sleep(Task self) {
  self->resume = tk_wup_tsk;
  tk_slp_tsk(TMO_FEVR);
}

inline static void SuspendOther(Task self) {
  self->resume = tk_rsm_tsk;
  tk_sus_tsk(self->id);
  tk_rel_wai(self->id);  // In case of task state is Blocked.
}

static void Suspend(Task self) {
  if (IsSuspended(self)) return;

  if (IsMyself(self))
    Sleep(self);
  else
    SuspendOther(self);
}

static void Resume(Task self) {
  if (IsSuspended(self)) {
    ResumeDelegate resume = self->resume;
    self->resume = NULL;
    resume(self->id);
  }
}

static void Delay(int time_in_milliseconds) { tk_dly_tsk(time_in_milliseconds); }

static const TaskMethodStruct kTheMethod = {
    .New = New, .Delete = Delete, .Run = Run, .Suspend = Suspend, .Resume = Resume, .Delay = Delay,
};

const TaskMethod task = &kTheMethod;
