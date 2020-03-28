// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "isr.h"

#include "isr_private.h"

static void Delete(Isr* self) { (*self)->impl->Delete(self); }

static void Enable(Isr self) { self->impl->Enable(self); }

static void Disable(Isr self) { self->impl->Disable(self); }

static const IsrInterfaceStruct kTheInterface = {
    .Delete = Delete,
    .Enable = Enable,
    .Disable = Disable,
};

const IsrInterface isr = &kTheInterface;
