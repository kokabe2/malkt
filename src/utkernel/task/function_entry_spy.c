// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "function_entry_spy.h"

static bool was_run;

static void TaskFunction(void) { was_run = true; }
static ActionDelegate Get(void) { return TaskFunction; }
static void Reset(void) { was_run = false; }
static bool WasRun(void) { return was_run; }
static const FunctionEntrySpyMethodStruct kTheMethod = {
    .Get = Get, .Reset = Reset, .WasRun = WasRun,
};
const FunctionEntrySpyMethod functionEntrySpy = &kTheMethod;
