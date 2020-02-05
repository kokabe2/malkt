// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_UTKERNEL_UTIL_SYSTEM_CALL_LOGGER_H_
#define SRC_UTKERNEL_UTIL_SYSTEM_CALL_LOGGER_H_

enum {
  kNonsenseParameter = 0xdeadbeef,
};

typedef struct {
  void (*Reset)(void);
  void (*Enter)(const char* system_call_name, int id);
  void (*Exit)(const char* system_call_name, int return_code);
  const char* (*Get)(void);
} SystemCallLoggerMethodStruct;
typedef const SystemCallLoggerMethodStruct* SystemCallLoggerMethod;

extern const SystemCallLoggerMethod systemCallLogger;

#endif  // SRC_UTKERNEL_UTIL_SYSTEM_CALL_LOGGER_H_
