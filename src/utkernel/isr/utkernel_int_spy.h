// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_UTKERNEL_ISR_UTKERNEL_INT_SPY_H_
#define SRC_UTKERNEL_ISR_UTKERNEL_INT_SPY_H_

#include "utkernel/utkernel.h"

typedef struct {
  void (*Reset)(void);
  ATR (*Attribute)(void);
  FP (*InterruptHandler)(void);
  INT (*Level)(void);
  void (*SetReturnCode)(int number, INT code);
} UtkernelIntSpyMethodStruct;
typedef const UtkernelIntSpyMethodStruct* UtkernelIntSpyMethod;

extern const UtkernelIntSpyMethod utkernelIntSpy;

#endif  // SRC_UTKERNEL_ISR_UTKERNEL_INT_SPY_H_
