// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "isr_spy.h"

#include <stddef.h>

#include "isr.h"

typedef struct IsrStruct {
  int dummy;  //
} IsrStruct;

static IsrStruct isr_dummy;
static int its_interrupt_nubmer;
static int its_level;
static InterruptDelegate its_interrupt_handler;

static void Reset(void) {
  its_interrupt_nubmer = 0;
  its_level = 0;
  its_interrupt_handler = NULL;
}

static int InterruptNumber(void) { return its_interrupt_nubmer; }

static int Level(void) { return its_level; }

static void RunHandler(void) { its_interrupt_handler(its_interrupt_nubmer); }

static const IsrSpyMethodStruct kSpyMethod = {
    .Reset = Reset,
    .InterruptNumber = InterruptNumber,
    .Level = Level,
    .RunHandler = RunHandler,
};

const IsrSpyMethod isrSpy = &kSpyMethod;

static Isr New(int interrupt_number, InterruptDelegate interrupt) {
  its_interrupt_nubmer = interrupt_number;
  its_interrupt_handler = interrupt;
  return &isr_dummy;
}

static void Delete(Isr* self) { Reset(); }

static void Enable(Isr self, int level) { its_level = level; }

static void Disable(Isr self) { its_level = 0; }

static const IsrMethodStruct kTheMethod = {
    .New = New,
    .Delete = Delete,
    .Enable = Enable,
    .Disable = Disable,
};

const IsrMethod isr = &kTheMethod;
