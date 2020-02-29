// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "utkernel_mpf_spy.h"

#include <string.h>

#include "../util/system_call_template.h"

typedef struct {
  void **p_blf;
  TMO tmout;
} GetMpfParameterStruct;

static ATR its_attribute;
static SZ its_block_count;
static SZ its_block_size;
static void *its_user_buffer;
static TMO its_timeout;

static int number_of_executions;
static INT return_codes[8];

static void Reset(void) {
  its_attribute = 0;
  its_block_count = 0;
  its_block_size = 0;
  its_user_buffer = NULL;
  its_timeout = 0;
  number_of_executions = 0;
  memset(return_codes, 0, sizeof(return_codes));
}
static ATR Attribute(void) { return its_attribute; }
static SZ BlockCount(void) { return its_block_count; }
static SZ BlockSize(void) { return its_block_size; }
static void *UserBuffer(void) { return its_user_buffer; }
static TMO Timout(void) { return its_timeout; }
static void SetReturnCode(int number, INT code) { return_codes[number_of_executions + number] = code; }
static const UtkernelMpfSpyMethodStruct kTheMethod = {
    .Reset = Reset,
    .Attribute = Attribute,
    .BlockCount = BlockCount,
    .BlockSize = BlockSize,
    .UserBuffer = UserBuffer,
    .Timout = Timout,
    .SetReturnCode = SetReturnCode,
};
const UtkernelMpfSpyMethod utkernelMpfSpy = &kTheMethod;

static void _tk_cre_mpf(const void *info) {
  T_CMPF *pk_cmpf = (T_CMPF *)info;
  its_attribute = pk_cmpf->mpfatr;
  its_block_count = pk_cmpf->mpfcnt;
  its_block_size = pk_cmpf->blfsz;
  its_user_buffer = pk_cmpf->bufptr;
}
ID tk_cre_mpf(CONST T_CMPF *pk_cmpf) {
  systemCallTemplate->SetReturnCode(return_codes[number_of_executions++]);
  return systemCallTemplate->Execute(__func__, _tk_cre_mpf, pk_cmpf);
}
inline static INT Template(const char *system_call_name, ID mpfid, ExecuteDelegate execute, const void *info) {
  systemCallTemplate->SetId(mpfid);
  systemCallTemplate->SetReturnCode(return_codes[number_of_executions++]);
  return systemCallTemplate->Execute(system_call_name, execute, info);
}
ER tk_del_mpf(ID mpfid) { return Template(__func__, mpfid, NULL, NULL); }
static void _tk_get_mpf(const void *info) {
  GetMpfParameterStruct *gmps = (GetMpfParameterStruct *)info;
  its_timeout = gmps->tmout;
  *gmps->p_blf = its_user_buffer;
}
ER tk_get_mpf(ID mpfid, void **p_blf, TMO tmout) {
  GetMpfParameterStruct gmps = {.p_blf = p_blf, .tmout = tmout};
  return Template(__func__, mpfid, _tk_get_mpf, &gmps);
}
ER tk_rel_mpf(ID mpfid, void *blk) { return Template(__func__, mpfid, NULL, NULL); }
