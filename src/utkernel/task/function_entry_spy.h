// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_UTKERNEL_TASK_FUNCTION_ENTRY_SPY_H_
#define SRC_UTKERNEL_TASK_FUNCTION_ENTRY_SPY_H_

#include <stdbool.h>

#include "task.h"

typedef struct {
  ActionDelegate (*Get)(void);
  void (*Reset)(void);
  bool (*WasRun)(void);
} FunctionEntrySpyMethodStruct;
typedef const FunctionEntrySpyMethodStruct* FunctionEntrySpyMethod;

extern const FunctionEntrySpyMethod functionEntrySpy;

#endif  // SRC_UTKERNEL_TASK_FUNCTION_ENTRY_SPY_H_
