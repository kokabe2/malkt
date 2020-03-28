// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "time_unit.h"

#include "uptime.h"
#include "utkernel/utkernel.h"

enum {
  kBaseTimeUnit = 1,
  kMillisecond = kBaseTimeUnit,
  kSecond = kMillisecond * 1000,
  kMinute = kSecond * 60,
  kHour = kMinute * 60,
};

static int64_t Now(Duration d) { return uptime->Get() / d; }

static void Sleep(Duration d) { tk_dly_tsk(d); }

static const TimeUnitMethodStruct kTheMethod = {
    .Millisecond = kMillisecond,
    .Second = kSecond,
    .Minute = kMinute,
    .Hour = kHour,
    .Now = Now,
    .Sleep = Sleep,
};

const TimeUnitMethod timeUnit = &kTheMethod;
