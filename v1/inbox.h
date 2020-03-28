// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef V1_INBOX_H_
#define V1_INBOX_H_

#include <stdbool.h>

typedef struct InboxStruct* Inbox;
typedef struct {
  void (*Delete)(Inbox* self);
  bool (*Post)(Inbox self, const void* message, int size);
  bool (*BlockingPost)(Inbox self, const void* message, int size);
  void* (*Get)(Inbox self);
  void* (*BlockingGet)(Inbox self);
} InboxInterfaceStruct;
typedef const InboxInterfaceStruct* InboxInterface;

extern const InboxInterface inbox;

#endif  // V1_INBOX_H_
