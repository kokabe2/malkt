// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_UPTIME_H_
#define V1_UPTIME_H_

#include <stdint.h>

typedef struct {
  uint64_t (*Get)(void);  // The order of milliseconds
} UptimeMethodStruct;
typedef const UptimeMethodStruct* UptimeMethod;

extern const UptimeMethod uptime;

#endif  // V1_UPTIME_H_
