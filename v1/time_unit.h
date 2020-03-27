// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_TIME_UNIT_H_
#define V1_TIME_UNIT_H_

#include <stdint.h>

typedef int64_t Duration;
typedef struct {
  const Duration Millisecond;
  const Duration Second;
  const Duration Minute;
  const Duration Hour;
  int64_t (*Now)(Duration d);
  void (*Sleep)(Duration d);
} TimeUnitMethodStruct;
typedef const TimeUnitMethodStruct* TimeUnitMethod;

extern const TimeUnitMethod timeUnit;

#endif  // V1_TIME_UNIT_H_
