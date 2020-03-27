// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_MEMORY_POOL_UTKERNEL_UTKERNEL_MPF_SPY_H_
#define SRC_MEMORY_POOL_UTKERNEL_UTKERNEL_MPF_SPY_H_

#include "utkernel/utkernel.h"

typedef struct {
  void (*Reset)(void);
  ATR (*Attribute)(void);
  SZ (*BlockCount)(void);
  SZ (*BlockSize)(void);
  void* (*UserBuffer)(void);
  TMO (*Timout)(void);
  void (*SetReturnCode)(int number, INT code);
} UtkernelMpfSpyMethodStruct;
typedef const UtkernelMpfSpyMethodStruct* UtkernelMpfSpyMethod;

extern const UtkernelMpfSpyMethod utkernelMpfSpy;

#endif  // SRC_MEMORY_POOL_UTKERNEL_UTKERNEL_MPF_SPY_H_
