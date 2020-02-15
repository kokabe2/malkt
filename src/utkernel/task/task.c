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

inline static bool Validate(ActionDelegate action, int priority,
                            int stack_size) {
  return action && priority >= kLowestTaskPriority &&
         priority <= kHighestTaskPriority && stack_size > 0 &&
         stack_size <= kMaxTaskStackSize;
}
static void TaskEntry(int unused, void* exinf) {
  Task self = (Task)exinf;
  self->action();
  tk_ext_tsk();
}
inline static int Reverse(int priority) {
  return kHighestTaskPriority - priority + kPriorityOffset;
}
inline static bool CreateTask(Task self, int priority, int stack_size) {
  T_CTSK packet = {.exinf = (void*)self,
                   .tskatr = (TA_HLNG | TA_RNG0),
                   .task = (FP)TaskEntry,
                   .itskpri = (PRI)Reverse(priority),
                   .stksz = (SZ)stack_size};
  return (self->id = tk_cre_tsk(&packet)) >= 0;
}
static Task New(ActionDelegate action, int priority, int stack_size) {
  if (!Validate(action, priority, stack_size)) return NULL;
  Task self = (Task)heap->New(sizeof(TaskStruct));
  if (!self) return self;
  self->action = action;
  if (!CreateTask(self, priority, stack_size)) heap->Delete((void**)&self);
  return self;
}
inline static bool IsMyself(int id) { return id == tk_get_tid(); }
inline static void KillMyself(Task* self) {
  heap->Delete((void**)self);
  tk_exd_tsk();
}
inline static void KillOther(Task* self) {
  tk_ter_tsk((*self)->id);  // No need to care about error.
  tk_del_tsk((*self)->id);
  heap->Delete((void**)self);
}
static void Delete(Task* self) {
  if (!self || !(*self)) return;
  if (IsMyself((*self)->id))
    KillMyself(self);
  else
    KillOther(self);
}
static void Run(Task self) {
  if (self) tk_sta_tsk(self->id, 0);  // No need to care about error.
}
inline static bool IsSuspended(Task self) { return self->resume; }
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
  if (!self || IsSuspended(self)) return;
  if (IsMyself(self->id))
    Sleep(self);
  else
    SuspendOther(self);
}
static void Resume(Task self) {
  if (!self || !IsSuspended(self)) return;
  ResumeDelegate resume = self->resume;
  self->resume = NULL;
  resume(self->id);
}
static void Delay(int time_in_milliseconds) {
  if (time_in_milliseconds > 0) tk_dly_tsk(time_in_milliseconds);
}
static const TaskMethodStruct kTheMethod = {
    .New = New,
    .Delete = Delete,
    .Run = Run,
    .Suspend = Suspend,
    .Resume = Resume,
    .Delay = Delay,
};
const TaskMethod task = &kTheMethod;
