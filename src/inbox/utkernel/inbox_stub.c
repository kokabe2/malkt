// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include <string.h>

#include "bleu/v1/heap.h"
#include "inbox.h"

typedef struct InboxStruct {
  void* last_message;
  bool got;
} InboxStruct;

static InboxStruct the_instance;

static Inbox New(int capacity) {
  the_instance.last_message = NULL;
  return &the_instance;
}

static void Delete(Inbox* self) { heap->Delete(&(*self)->last_message); }

static bool Post(Inbox self, const void* message, int size) {
  heap->Delete(&self->last_message);
  self->last_message = heap->New(size);
  memcpy(self->last_message, message, size);
  self->got = false;
  return true;
}

static bool BlockingPost(Inbox self, const void* message, int size) { return Post(self, message, size); }

static void* Get(Inbox self) {
  if (self->got) {
    return NULL;
  } else {
    self->got = true;
    return self->last_message;
  }
}

static void* BlockingGet(Inbox self) { return Get(self); }

static const InboxMethodStruct kTheMethod = {
    .New = New,
    .Delete = Delete,
    .Post = Post,
    .BlockingPost = BlockingPost,
    .Get = Get,
    .BlockingGet = BlockingGet,
};

const InboxMethod inbox = &kTheMethod;
