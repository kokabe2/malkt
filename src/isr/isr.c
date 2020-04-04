// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "isr.h"

static void Delete(Isr* self) { (*self)->Delete(self); }

static void Enable(Isr self) { self->Enable(self); }

static void Disable(Isr self) { self->Disable(self); }

static const IsrInterfaceStruct kTheInterface = {
    .Delete = Delete, .Enable = Enable, .Disable = Disable,
};

const IsrInterface isr = &kTheInterface;
