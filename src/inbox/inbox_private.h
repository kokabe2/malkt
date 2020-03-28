// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef SRC_INBOX_INBOX_PRIVATE_H_
#define SRC_INBOX_INBOX_PRIVATE_H_

#include "inbox.h"

typedef struct InboxStruct {
  InboxInterface impl;
  int id;
} InboxStruct;

#endif  // SRC_INBOX_INBOX_PRIVATE_H_
