// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "system_call_logger.h"

#include <stdio.h>
#include <string.h>

static int nest_level;
static int log_offset;
static char log[128];

static void Reset(void) {
  nest_level = 0;
  log_offset = 0;
  memset(log, 0, sizeof(log));
}
inline static int free_space(void) { return sizeof(log) - log_offset; }
inline static void AddCount(const char* plus_or_minus) {
  for (int i = 0; i < nest_level; ++i)
    log_offset += snprintf(&log[log_offset], free_space(), "%s", plus_or_minus);
}
inline static void AddLog(const char* system_call_name, int i) {
  const char* format = i != kNonsenseParameter ? " %s (%d)\n" : " %s\n";
  log_offset +=
      snprintf(&log[log_offset], free_space(), format, system_call_name, i);
}
static void Enter(const char* system_call_name, int id) {
  ++nest_level;
  AddCount("+");
  AddLog(system_call_name, id);
}
static void Exit(const char* system_call_name, int return_code) {
  AddCount("-");
  AddLog(system_call_name, return_code);
  --nest_level;
}
static const char* Get(void) { return log; }
static const SystemCallLoggerMethodStruct kTheMethod = {
    .Reset = Reset, .Enter = Enter, .Exit = Exit, .Get = Get,
};
const SystemCallLoggerMethod systemCallLogger = &kTheMethod;
