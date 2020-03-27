// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "utkernel_mpl_spy.h"

#include <string.h>

#include "../../util/system_call_template.h"

typedef struct {
  SZ blksz;
  void **p_blk;
  TMO tmout;
} GetMplParameterStruct;

static ATR its_attribute;
static SZ its_capacity;
static SZ its_block_size;
static TMO its_timeout;
static UB dummy_buffer[128];

static int number_of_executions;
static INT return_codes[8];

static void Reset(void) {
  its_attribute = 0;
  its_capacity = 0;
  its_block_size = 0;
  its_timeout = 0;
  number_of_executions = 0;
  memset(return_codes, 0, sizeof(return_codes));
}

static ATR Attribute(void) { return its_attribute; }

static SZ Capacity(void) { return its_capacity; }

static SZ BlockSize(void) { return its_block_size; }

static TMO Timout(void) { return its_timeout; }

static void SetReturnCode(int number, INT code) { return_codes[number_of_executions + number] = code; }

static const UtkernelMplSpyMethodStruct kTheMethod = {
    .Reset = Reset,
    .Attribute = Attribute,
    .Capacity = Capacity,
    .BlockSize = BlockSize,
    .Timout = Timout,
    .SetReturnCode = SetReturnCode,
};

const UtkernelMplSpyMethod utkernelMplSpy = &kTheMethod;

static void _tk_cre_mpl(const void *info) {
  T_CMPL *pk_cmpl = (T_CMPL *)info;
  its_attribute = pk_cmpl->mplatr;
  its_capacity = pk_cmpl->mplsz;
}

ID tk_cre_mpl(CONST T_CMPL *pk_cmpl) {
  systemCallTemplate->SetReturnCode(return_codes[number_of_executions++]);
  return systemCallTemplate->Execute(__func__, _tk_cre_mpl, pk_cmpl);
}

inline static INT Template(const char *system_call_name, ID mplid, ExecuteDelegate execute, const void *info) {
  systemCallTemplate->SetId(mplid);
  systemCallTemplate->SetReturnCode(return_codes[number_of_executions++]);
  return systemCallTemplate->Execute(system_call_name, execute, info);
}

ER tk_del_mpl(ID mplid) { return Template(__func__, mplid, NULL, NULL); }

static void _tk_get_mpl(const void *info) {
  GetMplParameterStruct *gmps = (GetMplParameterStruct *)info;
  its_timeout = gmps->tmout;
  its_block_size = gmps->blksz;
  *gmps->p_blk = dummy_buffer;
}

ER tk_get_mpl(ID mplid, SZ blksz, void **p_blk, TMO tmout) {
  GetMplParameterStruct gmps = {.blksz = blksz, .p_blk = p_blk, .tmout = tmout};
  return Template(__func__, mplid, _tk_get_mpl, &gmps);
}

ER tk_rel_mpl(ID mplid, void *blk) { return Template(__func__, mplid, NULL, NULL); }
