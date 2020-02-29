// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "utkernel_cyc_spy.h"

#include <string.h>

#include "../util/system_call_template.h"

static void *its_exinf;
static ATR its_attribute;
static FP its_timer;
static RELTIM its_cycle_time;
static RELTIM its_cycle_phase;

static int number_of_executions;
static INT return_codes[8];

static void Reset(void) {
  its_exinf = NULL;
  its_attribute = 0;
  its_timer = NULL;
  its_cycle_time = 0;
  number_of_executions = 0;
  memset(return_codes, 0, sizeof(return_codes));
}
static ATR Attribute(void) { return its_attribute; }
static RELTIM CycleTime(void) { return its_cycle_time; }
static RELTIM CyclePhase(void) { return its_cycle_phase; }
static void RunTimer(void) { its_timer(its_exinf); }
static void SetReturnCode(int number, INT code) { return_codes[number_of_executions + number] = code; }
static const UtkernelCycSpyMethodStruct kTheMethod = {
    .Reset = Reset,
    .Attribute = Attribute,
    .CycleTime = CycleTime,
    .CyclePhase = CyclePhase,
    .RunTimer = RunTimer,
    .SetReturnCode = SetReturnCode,
};
const UtkernelCycSpyMethod utkernelCycSpy = &kTheMethod;

static void _tk_cre_cyc(const void *info) {
  T_CCYC *pk_ccyc = (T_CCYC *)info;
  its_exinf = pk_ccyc->exinf;
  its_attribute = pk_ccyc->cycatr;
  its_timer = pk_ccyc->cychdr;
  its_cycle_time = pk_ccyc->cyctim;
  its_cycle_phase = pk_ccyc->cycphs;
}
ID tk_cre_cyc(CONST T_CCYC *pk_ccyc) {
  systemCallTemplate->SetReturnCode(return_codes[number_of_executions++]);
  return systemCallTemplate->Execute(__func__, _tk_cre_cyc, pk_ccyc);
}
inline static INT Template(const char *system_call_name, ID cycid) {
  systemCallTemplate->SetId(cycid);
  systemCallTemplate->SetReturnCode(return_codes[number_of_executions++]);
  return systemCallTemplate->Execute(system_call_name, NULL, NULL);
}
ER tk_del_cyc(ID cycid) { return Template(__func__, cycid); }
ER tk_sta_cyc(ID cycid) { return Template(__func__, cycid); }
ER tk_stp_cyc(ID cycid) { return Template(__func__, cycid); }
