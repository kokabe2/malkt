// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_UTKERNEL_TASK_UTKERNEL_TSK_SPY_H_
#define SRC_UTKERNEL_TASK_UTKERNEL_TSK_SPY_H_

#include "utkernel/utkernel.h"

#define TTS_NONE 0x00000000

typedef struct {
  void (*Reset)(void);
  ATR (*Attribute)(void);
  PRI (*Priority)(void);
  SZ (*StackSize)(void);
  TMO (*Timeout)(void);
  RELTIM (*DelayTime)(void);
  void (*SetReturnCode)(int number, INT code);
} UtkernelTskSpyMethodStruct;
typedef const UtkernelTskSpyMethodStruct* UtkernelTskSpyMethod;

extern const UtkernelTskSpyMethod utkernelTskSpy;

#endif  // SRC_UTKERNEL_TASK_UTKERNEL_TSK_SPY_H_
