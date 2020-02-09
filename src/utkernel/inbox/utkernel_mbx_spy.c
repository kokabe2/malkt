// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "utkernel_mbx_spy.h"

#include <stdint.h>
#include <string.h>

#include "../util/system_call_template.h"

typedef struct {
  T_MSG** ppk_msg;
  TMO tmout;
} RcvMbxParameterStruct;

static ATR its_attribute;
static TMO its_timeout;
static void* its_last_message;

static int number_of_executions;
static INT return_codes[8];

static void Reset(void) {
  its_attribute = 0;
  its_timeout = 0;
  its_last_message = NULL;
  number_of_executions = 0;
  memset(return_codes, 0, sizeof(return_codes));
}
static ATR Attribute(void) { return its_attribute; }
static TMO Timeout(void) { return its_timeout; }
static void* LastMessage(void) {
  return (void*)((uintptr_t)its_last_message + sizeof(T_MSG));
}
static void SetReturnCode(int number, INT code) {
  return_codes[number_of_executions + number] = code;
}
static const UtkernelMbxSpyMethodStruct kTheMethod = {
    .Reset = Reset,
    .Attribute = Attribute,
    .Timeout = Timeout,
    .LastMessage = LastMessage,
    .SetReturnCode = SetReturnCode,
};
const UtkernelMbxSpyMethod utkernelMbxSpy = &kTheMethod;

static void _tk_cre_mbx(const void* info) {
  T_CMBX* pk_cmbx = (T_CMBX*)info;
  its_attribute = pk_cmbx->mbxatr;
}
ID tk_cre_mbx(CONST T_CMBX* pk_cmbx) {
  systemCallTemplate->SetReturnCode(return_codes[number_of_executions++]);
  return systemCallTemplate->Execute(__func__, _tk_cre_mbx, pk_cmbx);
}
inline static INT Template(const char* system_call_name, ID mbxid,
                           ExecuteDelegate execute, const void* info) {
  systemCallTemplate->SetId(mbxid);
  systemCallTemplate->SetReturnCode(return_codes[number_of_executions++]);
  return systemCallTemplate->Execute(system_call_name, execute, info);
}
ER tk_del_mbx(ID mbxid) { return Template(__func__, mbxid, NULL, NULL); }
static void _tk_snd_mbx(const void* info) {
  T_MSG* pk_msg = (T_MSG*)info;
  its_last_message = pk_msg;
}
ER tk_snd_mbx(ID mbxid, T_MSG* pk_msg) {
  return Template(__func__, mbxid, _tk_snd_mbx, pk_msg);
}
static void _tk_rcv_mbx(const void* info) {
  RcvMbxParameterStruct* rmps = (RcvMbxParameterStruct*)info;
  its_timeout = rmps->tmout;
  *rmps->ppk_msg = its_last_message;
}
ER tk_rcv_mbx(ID mbxid, T_MSG** ppk_msg, TMO tmout) {
  RcvMbxParameterStruct rmps = {.ppk_msg = ppk_msg, .tmout = tmout};
  return Template(__func__, mbxid, _tk_rcv_mbx, &rmps);
}
