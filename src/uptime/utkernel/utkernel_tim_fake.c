// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "utkernel/utkernel.h"

static SYSTIM system_time;

ER tk_set_tim(CONST SYSTIM *pk_tim) {
  if (!pk_tim) return E_PAR;
  system_time.hi = pk_tim->hi;
  system_time.lo = pk_tim->lo;
  return E_OK;
}

ER tk_get_tim(SYSTIM *pk_tim) {
  if (!pk_tim) return E_PAR;
  pk_tim->hi = system_time.hi;
  pk_tim->lo = system_time.lo;
  return E_OK;
}
