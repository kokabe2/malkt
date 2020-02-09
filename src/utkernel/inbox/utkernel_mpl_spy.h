// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_UTKERNEL_INBOX_UTKERNEL_MPL_SPY_H_
#define SRC_UTKERNEL_INBOX_UTKERNEL_MPL_SPY_H_

#include <stdbool.h>

#include "utkernel/utkernel.h"

typedef struct {
  void (*Reset)(void);
  ATR (*Attribute)(void);
  SZ (*Capacity)(void);
  SZ (*BlockSize)(void);
  TMO (*Timout)(void);
  void (*SetReturnCode)(int number, INT code);
} UtkernelMplSpyMethodStruct;
typedef const UtkernelMplSpyMethodStruct* UtkernelMplSpyMethod;

extern const UtkernelMplSpyMethod utkernelMplSpy;

#endif  // SRC_UTKERNEL_INBOX_UTKERNEL_MPL_SPY_H_
