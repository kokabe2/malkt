// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_ISR_ISR_PRIVATE_H_
#define SRC_ISR_ISR_PRIVATE_H_

#include "isr.h"

typedef struct IsrStruct {
  IsrInterface impl;
  int number;
  int level;
} IsrStruct;

#endif  // SRC_ISR_ISR_PRIVATE_H_
