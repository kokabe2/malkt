// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_UTKERNEL_UTIL_SYSTEM_CALL_TEMPLATE_H_
#define SRC_UTKERNEL_UTIL_SYSTEM_CALL_TEMPLATE_H_

typedef void (*ExecuteDelegate)(const void* info);
typedef struct {
  void (*SetId)(int id);
  void (*SetReturnCode)(int return_code);
  int (*Execute)(const char* system_call_name, ExecuteDelegate execute,
                 const void* info);
} SystemCallTemplateMethodStruct;
typedef const SystemCallTemplateMethodStruct* SystemCallTemplateMethod;

extern const SystemCallTemplateMethod systemCallTemplate;

#endif  // SRC_UTKERNEL_UTIL_SYSTEM_CALL_TEMPLATE_H_
