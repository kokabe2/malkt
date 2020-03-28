// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_ISR_H_
#define V1_ISR_H_

typedef struct IsrStruct* Isr;
typedef struct {
  void (*Delete)(Isr* self);
  void (*Enable)(Isr self);
  void (*Disable)(Isr self);
} IsrInterfaceStruct;
typedef const IsrInterfaceStruct* IsrInterface;

extern const IsrInterface isr;

#endif  // V1_ISR_H_
