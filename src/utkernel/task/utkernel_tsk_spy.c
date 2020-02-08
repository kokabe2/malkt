// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "utkernel_tsk_spy.h"

#include <string.h>

#include "../util/system_call_logger.h"

static void *its_exinf;
static ATR its_attribute;
static FP its_task;
static PRI its_priority;
static SZ its_stack_size;
static TMO its_timeout;
static RELTIM its_delay_time;

static int number_of_executions;
static INT return_codes[8];

inline static void ResetTaskInfo(void) {
  its_exinf = NULL;
  its_attribute = 0;
  its_task = NULL;
  its_priority = 0;
  its_stack_size = 0;
  its_timeout = 0;
  its_delay_time = 0;
}
static void Reset(void) {
  ResetTaskInfo();
  number_of_executions = 0;
  memset(return_codes, 0, sizeof(return_codes));
}
static ATR Attribute(void) { return its_attribute; }
static PRI Priority(void) { return its_priority; }
static SZ StackSize(void) { return its_stack_size; }
static TMO Timeout(void) { return its_timeout; }
static RELTIM DelayTime(void) { return its_delay_time; }
static void SetReturnCode(int number, INT code) {
  return_codes[number_of_executions + number] = code;
}
static const UtkernelTskSpyMethodStruct kTheMethod = {
    .Reset = Reset,
    .Attribute = Attribute,
    .Priority = Priority,
    .StackSize = StackSize,
    .Timeout = Timeout,
    .DelayTime = DelayTime,
    .SetReturnCode = SetReturnCode,
};
const UtkernelTskSpyMethod utkernelTskSpy = &kTheMethod;

inline static void Enter(const char *system_call_name, INT id) {
  systemCallLogger->Enter(system_call_name, id);
}
inline static INT Exit(const char *system_call_name) {
  int return_code = return_codes[number_of_executions++];
  systemCallLogger->Exit(system_call_name, return_code);
  return return_code;
}
static INT Template(const char *system_call_name, INT id,
                    void (*execute)(void)) {
  Enter(system_call_name, id);
  if (execute) execute();
  return Exit(system_call_name);
}
inline static bool IsErrorCodeSet(void) {
  return return_codes[number_of_executions] < 0;
}
static void _tk_cre_tsk(void) {
  if (IsErrorCodeSet()) ResetTaskInfo();
}
ID tk_cre_tsk(CONST T_CTSK *pk_ctsk) {
  its_exinf = pk_ctsk->exinf;
  its_attribute = pk_ctsk->tskatr;
  its_task = pk_ctsk->task;
  its_priority = pk_ctsk->itskpri;
  its_stack_size = pk_ctsk->stksz;
  return Template(__func__, kNonsenseParameter, _tk_cre_tsk);
}
ER tk_del_tsk(ID tskid) { return Template(__func__, tskid, NULL); }
static void _tk_sta_tsk(void) {
  if (!IsErrorCodeSet()) its_task(0, its_exinf);
}
ER tk_sta_tsk(ID tskid, INT stacd) {
  return Template(__func__, tskid, _tk_sta_tsk);
}
void tk_ext_tsk(void) {
  SetReturnCode(0, kNonsenseParameter);
  Template(__func__, kNonsenseParameter, NULL);
}
void tk_exd_tsk(void) {
  SetReturnCode(0, kNonsenseParameter);
  Template(__func__, kNonsenseParameter, NULL);
}
ER tk_ter_tsk(ID tskid) { return Template(__func__, tskid, NULL); }
ER tk_slp_tsk(TMO tmout) {
  its_timeout = tmout;
  return Template(__func__, kNonsenseParameter, NULL);
}
ER tk_wup_tsk(ID tskid) { return Template(__func__, tskid, NULL); }
ER tk_rel_wai(ID tskid) { return Template(__func__, tskid, NULL); }
ER tk_sus_tsk(ID tskid) { return Template(__func__, tskid, NULL); }
ER tk_rsm_tsk(ID tskid) { return Template(__func__, tskid, NULL); }
ER tk_dly_tsk(RELTIM dlytim) {
  its_delay_time = dlytim;
  return Template(__func__, kNonsenseParameter, NULL);
}
ID tk_get_tid(void) { return Template(__func__, kNonsenseParameter, NULL); }
