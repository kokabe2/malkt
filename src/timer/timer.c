// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "timer.h"

static void Delete(Timer* self) { (*self)->Delete(self); }

static void Pause(Timer self) { self->Pause(self); }

static void Resume(Timer self) { self->Resume(self); }

static const TimerInterfaceStruct kTheInterface = {
    .Delete = Delete, .Pause = Pause, .Resume = Resume,
};

const TimerInterface timer = &kTheInterface;
