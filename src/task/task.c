// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "task.h"

static void Delete(Task* self) { (*self)->Delete(self); }

static void Run(Task self) { self->Run(self); }

static void Suspend(Task self) { self->Suspend(self); }

static void Resume(Task self) { self->Resume(self); }

static const TaskInterfaceStruct kTheInterface = {
    .Delete = Delete, .Run = Run, .Suspend = Suspend, .Resume = Resume,
};

const TaskInterface task = &kTheInterface;
