// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_BASIC_ISR_H_
#define V1_BASIC_ISR_H_

#include "isr.h"

typedef void (*InterruptDelegate)(int interrupt_number);
typedef struct {
  Isr (*New)(int interrupt_number, int interrupt_level, InterruptDelegate delegate);
} BasicIsrMethodStruct;
typedef const BasicIsrMethodStruct* BasicIsrMethod;

extern const BasicIsrMethod basicIsr;

#endif  // V1_BASIC_ISR_H_
