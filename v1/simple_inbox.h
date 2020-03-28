// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_SIMPLE_INBOX_H_
#define V1_SIMPLE_INBOX_H_

#include "inbox.h"

enum {
  kMaxInboxCapacity = 4 * 1024,
};

typedef struct {
  Inbox (*New)(int capacity);  //
} SimpleInboxMethodStruct;
typedef const SimpleInboxMethodStruct* SimpleInboxMethod;

extern const SimpleInboxMethod simpleInbox;

#endif  // V1_SIMPLE_INBOX_H_
