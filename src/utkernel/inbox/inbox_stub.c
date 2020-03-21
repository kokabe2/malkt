// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include <string.h>

#include "bleu/v1/heap.h"
#include "inbox.h"

typedef struct InboxStruct {
  void* last_message;
} InboxStruct;

static InboxStruct the_instance;

static Inbox New(int capacity) { return &the_instance; }

static void Delete(Inbox* self) { heap->Delete((*self)->last_message); }

static bool Post(Inbox self, const void* message, int size) {
  Delete(&self);
  self->last_message = heap->New(size);
  memcpy(self->last_message, message, size);
  return true;
}

static bool BlockingPost(Inbox self, const void* message, int size) { return Post(self, message, size); }

static void* Get(Inbox self) { return self->last_message; }

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
