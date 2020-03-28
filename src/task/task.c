// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "task.h"

#include "task_private.h"

static void Delete(Task* self) { (*self)->impl->Delete(self); }

static void Run(Task self) { self->impl->Run(self); }

static void Suspend(Task self) { self->impl->Suspend(self); }

static void Resume(Task self) { self->impl->Resume(self); }

static const TaskInterfaceStruct kTheInterface = {
    .Delete = Delete,
    .Run = Run,
    .Suspend = Suspend,
    .Resume = Resume,
};

const TaskInterface task = &kTheInterface;
