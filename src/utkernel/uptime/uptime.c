// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "uptime.h"

#include "utkernel/utkernel.h"

static uint64_t Get(void) {
  SYSTIM packet;
  tk_get_tim(&packet);
  return ((uint64_t)packet.hi << 32) + packet.lo;
}

static void Set(uint64_t milliseconds) {
  SYSTIM packet = {.hi = (W)(milliseconds >> 32), .lo = (UW)(milliseconds & 0x00000000FFFFFFFF)};
  tk_set_tim(&packet);
}

static const UptimeMethodStruct kTheMethod = {
    .Get = Get,
    .Set = Set,
};

const UptimeMethod uptime = &kTheMethod;
