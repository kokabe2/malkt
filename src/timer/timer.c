// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "timer.h"

#include "timer_private.h"

static void Delete(Timer* self) { (*self)->impl->Delete(self); }

static void Pause(Timer self) { self->impl->Pause(self); }

static void Resume(Timer self) { self->impl->Resume(self); }

static const TimerInterfaceStruct kTheInterface = {
    .Delete = Delete,
    .Pause = Pause,
    .Resume = Resume,
};

const TimerInterface timer = &kTheInterface;
