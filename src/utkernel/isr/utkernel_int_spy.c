// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "utkernel_int_spy.h"

#include <string.h>

#include "../util/system_call_template.h"

static ATR its_attribute;
static FP its_interrupt_handler;
static INT its_level;

static int number_of_executions;
static INT return_codes[8];

static void Reset(void) {
  its_attribute = 0;
  its_interrupt_handler = NULL;
  its_level = 0;
  number_of_executions = 0;
  memset(return_codes, 0, sizeof(return_codes));
}

static ATR Attribute(void) { return its_attribute; }

static FP InterruptHandler(void) { return its_interrupt_handler; }

static INT Level(void) { return its_level; }

static void SetReturnCode(int number, INT code) { return_codes[number_of_executions + number] = code; }

static const UtkernelIntSpyMethodStruct kTheMethod = {
    .Reset = Reset,
    .Attribute = Attribute,
    .InterruptHandler = InterruptHandler,
    .Level = Level,
    .SetReturnCode = SetReturnCode,
};

const UtkernelIntSpyMethod utkernelIntSpy = &kTheMethod;

static void _tk_def_int(const void *info) {
  T_DINT *pk_dint = (T_DINT *)info;
  its_attribute = pk_dint->intatr;
  its_interrupt_handler = pk_dint->inthdr;
}

ER tk_def_int(UINT intno, CONST T_DINT *pk_dint) {
  systemCallTemplate->SetId((int)intno);
  systemCallTemplate->SetReturnCode(return_codes[number_of_executions++]);
  return systemCallTemplate->Execute(__func__, _tk_def_int, pk_dint);
}

static void _EnableInt(const void *info) {
  INT *level = (INT *)info;
  its_level = *level;
}

void EnableInt(UINT intno, INT level) {
  systemCallTemplate->SetId((int)intno);
  systemCallTemplate->Execute(__func__, _EnableInt, &level);
}

void DisableInt(UINT intno) {
  systemCallTemplate->SetId((int)intno);
  systemCallTemplate->Execute(__func__, NULL, NULL);
}
