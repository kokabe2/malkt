// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_ISR_H_
#define V1_ISR_H_

typedef void (*InterruptDelegate)(int interrupt_number);
typedef struct IsrStruct* Isr;
typedef struct {
  Isr (*New)(int interrupt_number, InterruptDelegate interrupt);
  void (*Delete)(Isr* self);
  void (*Enable)(Isr self, int level);
  void (*Disable)(Isr self);
} IsrMethodStruct;
typedef const IsrMethodStruct* IsrMethod;

extern const IsrMethod isr;

#endif  // V1_ISR_H_
