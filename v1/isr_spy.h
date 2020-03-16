// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_ISR_SPY_H_
#define V1_ISR_SPY_H_

typedef struct {
  void (*Reset)(void);
  int (*InterruptNumber)(void);
  int (*Level)(void);
  void (*RunHandler)(void);
} IsrSpyMethodStruct;
typedef const IsrSpyMethodStruct* IsrSpyMethod;

extern const IsrSpyMethod isrSpy;

#endif  // V1_ISR_SPY_H_
