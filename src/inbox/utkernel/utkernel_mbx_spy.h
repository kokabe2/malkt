// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_INBOX_UTKERNEL_UTKERNEL_MBX_SPY_H_
#define SRC_INBOX_UTKERNEL_UTKERNEL_MBX_SPY_H_

#include "utkernel/utkernel.h"

typedef struct {
  void (*Reset)(void);
  ATR (*Attribute)(void);
  TMO (*Timeout)(void);
  void* (*LastMessage)(void);
  void (*SetReturnCode)(int number, INT code);
} UtkernelMbxSpyMethodStruct;
typedef const UtkernelMbxSpyMethodStruct* UtkernelMbxSpyMethod;

extern const UtkernelMbxSpyMethod utkernelMbxSpy;

#endif  // SRC_INBOX_UTKERNEL_UTKERNEL_MBX_SPY_H_
