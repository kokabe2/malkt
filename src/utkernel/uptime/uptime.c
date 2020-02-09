// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "uptime.h"

#include "utkernel/utkernel.h"

static uint64_t Get(void) {
  SYSTIM packet;
  tk_get_tim(&packet);
  return ((uint64_t)packet.hi << 32) + packet.lo;
}
static const UptimeMethodStruct kTheMethod = {
    .Get = Get,
};
const UptimeMethod uptime = &kTheMethod;
