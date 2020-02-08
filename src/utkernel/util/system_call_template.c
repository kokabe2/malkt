// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "system_call_template.h"

#include <stdbool.h>

#include "system_call_logger.h"

static int its_id = kNonsenseParameter;
static int its_return_code = kNonsenseParameter;

static void SetId(int id) { its_id = id; }
static void SetReturnCode(int return_code) { its_return_code = return_code; }
inline static bool IsNotError(int return_code) {
  return return_code == kNonsenseParameter || return_code >= 0;
}
inline static void ResetParameters(void) {
  its_id = kNonsenseParameter;
  its_return_code = kNonsenseParameter;
}
static int Execute(const char* system_call_name, ExecuteDelegate execute,
                   const void* info) {
  int id = its_id;
  int return_code = its_return_code;
  ResetParameters();
  systemCallLogger->Enter(system_call_name, id);
  if (execute && IsNotError(return_code)) execute(info);
  systemCallLogger->Exit(system_call_name, return_code);
  return return_code;
}
static const SystemCallTemplateMethodStruct kTheMethod = {
    .SetId = SetId, .SetReturnCode = SetReturnCode, .Execute = Execute,
};
const SystemCallTemplateMethod systemCallTemplate = &kTheMethod;
