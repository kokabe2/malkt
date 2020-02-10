// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_UTKERNEL_TIMER_UTKERNEL_CYC_SPY_H_
#define SRC_UTKERNEL_TIMER_UTKERNEL_CYC_SPY_H_

#include "utkernel/utkernel.h"

typedef struct {
  void (*Reset)(void);
  ATR (*Attribute)(void);
  RELTIM (*CycleTime)(void);
  RELTIM (*CyclePhase)(void);
  void (*SetReturnCode)(int number, INT code);
} UtkernelCycSpyMethodStruct;
typedef const UtkernelCycSpyMethodStruct* UtkernelCycSpyMethod;

extern const UtkernelCycSpyMethod utkernelCycSpy;

#endif  // SRC_UTKERNEL_TIMER_UTKERNEL_CYC_SPY_H_
