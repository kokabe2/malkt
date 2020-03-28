// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "inbox.h"

#include "inbox_private.h"

static void Delete(Inbox* self) { (*self)->impl->Delete(self); }

static bool Post(Inbox self, const void* message, int size) { return self->impl->Post(self, message, size); }

static bool BlockingPost(Inbox self, const void* message, int size) {
  return self->impl->BlockingPost(self, message, size);
}

static void* Get(Inbox self) { return self->impl->Get(self); }

static void* BlockingGet(Inbox self) { return self->impl->BlockingGet(self); }

static const InboxInterfaceStruct kTheInterface = {
    .Delete = Delete,
    .Post = Post,
    .BlockingPost = BlockingPost,
    .Get = Get,
    .BlockingGet = BlockingGet,
};

const InboxInterface inbox = &kTheInterface;
