// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_UPTIME_H_
#define V1_UPTIME_H_

#include <stdint.h>

typedef struct {
  int64_t (*Get)(void);
  void (*Set)(int64_t milliseconds);
} UptimeMethodStruct;
typedef const UptimeMethodStruct* UptimeMethod;

extern const UptimeMethod uptime;

#endif  // V1_UPTIME_H_
