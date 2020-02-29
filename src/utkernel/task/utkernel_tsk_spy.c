// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "utkernel_tsk_spy.h"

#include <string.h>

#include "../util/system_call_template.h"

static void *its_exinf;
static ATR its_attribute;
static FP its_task;
static PRI its_priority;
static SZ its_stack_size;
static TMO its_timeout;
static RELTIM its_delay_time;

static int number_of_executions;
static INT return_codes[8];

static void Reset(void) {
  its_exinf = NULL;
  its_attribute = 0;
  its_task = NULL;
  its_priority = 0;
  its_stack_size = 0;
  its_timeout = 0;
  its_delay_time = 0;
  number_of_executions = 0;
  memset(return_codes, 0, sizeof(return_codes));
}
static ATR Attribute(void) { return its_attribute; }
static PRI Priority(void) { return its_priority; }
static SZ StackSize(void) { return its_stack_size; }
static TMO Timeout(void) { return its_timeout; }
static RELTIM DelayTime(void) { return its_delay_time; }
static void SetReturnCode(int number, INT code) { return_codes[number_of_executions + number] = code; }
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

static void _tk_cre_tsk(const void *info) {
  T_CTSK *pk_ctsk = (T_CTSK *)info;
  its_exinf = pk_ctsk->exinf;
  its_attribute = pk_ctsk->tskatr;
  its_task = pk_ctsk->task;
  its_priority = pk_ctsk->itskpri;
  its_stack_size = pk_ctsk->stksz;
}
inline static INT TemplateWithoutId(const char *system_call_name, ExecuteDelegate execute, const void *info) {
  systemCallTemplate->SetReturnCode(return_codes[number_of_executions++]);
  return systemCallTemplate->Execute(system_call_name, execute, info);
}
ID tk_cre_tsk(CONST T_CTSK *pk_ctsk) { return TemplateWithoutId(__func__, _tk_cre_tsk, pk_ctsk); }
inline static INT Template(const char *system_call_name, ID tskid, ExecuteDelegate execute, const void *info) {
  systemCallTemplate->SetId(tskid);
  systemCallTemplate->SetReturnCode(return_codes[number_of_executions++]);
  return systemCallTemplate->Execute(system_call_name, execute, info);
}
ER tk_del_tsk(ID tskid) { return Template(__func__, tskid, NULL, NULL); }
static void _tk_sta_tsk(const void *unused) { its_task(0, its_exinf); }
ER tk_sta_tsk(ID tskid, INT stacd) { return Template(__func__, tskid, _tk_sta_tsk, NULL); }
void tk_ext_tsk(void) { systemCallTemplate->Execute(__func__, NULL, NULL); }
void tk_exd_tsk(void) { systemCallTemplate->Execute(__func__, NULL, NULL); }
ER tk_ter_tsk(ID tskid) { return Template(__func__, tskid, NULL, NULL); }
void _tk_slp_tsk(const void *info) {
  TMO *tmout = (TMO *)info;
  its_timeout = *tmout;
}
ER tk_slp_tsk(TMO tmout) { return TemplateWithoutId(__func__, _tk_slp_tsk, &tmout); }
ER tk_wup_tsk(ID tskid) { return Template(__func__, tskid, NULL, NULL); }
ER tk_rel_wai(ID tskid) { return Template(__func__, tskid, NULL, NULL); }
ER tk_sus_tsk(ID tskid) { return Template(__func__, tskid, NULL, NULL); }
ER tk_rsm_tsk(ID tskid) { return Template(__func__, tskid, NULL, NULL); }
void _tk_dly_tsk(const void *info) {
  RELTIM *dlytim = (RELTIM *)info;
  its_delay_time = *dlytim;
}
ER tk_dly_tsk(RELTIM dlytim) { return TemplateWithoutId(__func__, _tk_dly_tsk, &dlytim); }
ID tk_get_tid(void) { return TemplateWithoutId(__func__, NULL, NULL); }
