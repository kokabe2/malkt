// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "simple_inbox.h"

#include <stdint.h>
#include <string.h>

#include "bleu/v1/heap.h"
#include "utkernel/utkernel.h"

typedef struct {
  InboxInterfaceStruct impl;
  int mbx_id;
  int mpl_id;
  void* last_mail;
} SimpleInboxStruct, *SimpleInbox;
typedef void* (*ComposeDelegate)(SimpleInbox self, const void* message, int size);
typedef bool (*GetDelegate)(SimpleInbox self);

static const int kHeaderSize = sizeof(T_MSG);

static void Delete(Inbox* base) {
  SimpleInbox self = (SimpleInbox)*base;
  tk_del_mbx(self->mbx_id);
  tk_del_mpl(self->mpl_id);
  heap->Delete((void**)base);
}

inline static void SendMail(SimpleInbox self, void* mail) { tk_snd_mbx(self->mbx_id, (T_MSG*)mail); }

inline static bool PostTemplate(SimpleInbox self, const void* message, int size, ComposeDelegate compose) {
  void* mail = compose(self, message, size);
  if (mail != NULL) {
    SendMail(self, mail);
    return true;
  } else {
    return false;
  }
}

inline static void* GetMemoryBlock(SimpleInbox self, int size, TMO timeout) {
  void* mail;
  return tk_get_mpl(self->mpl_id, kHeaderSize + size, &mail, timeout) == E_OK ? mail : NULL;
}

inline static void* ExtractMessage(void* mail) { return (void*)((uintptr_t)mail + kHeaderSize); }

inline static void EditMail(void* mail, const void* message, int size) { memcpy(ExtractMessage(mail), message, size); }

static void* ComposeMail(SimpleInbox self, const void* message, int size) {
  void* mail = GetMemoryBlock(self, size, TMO_POL);
  if (mail != NULL) EditMail(mail, message, size);
  return mail;
}

static bool Post(Inbox self, const void* message, int size) {
  return PostTemplate((SimpleInbox)self, message, size, ComposeMail);
}

static void* BlockingComposeMail(SimpleInbox self, const void* message, int size) {
  void* mail = GetMemoryBlock(self, size, TMO_FEVR);
  if (mail != NULL) EditMail(mail, message, size);
  return mail;
}

static bool BlockingPost(Inbox self, const void* message, int size) {
  return PostTemplate((SimpleInbox)self, message, size, BlockingComposeMail);
}

inline static void DeleteLastMailIfNeeded(SimpleInbox self) {
  if (self->last_mail != NULL) {
    tk_rel_mpl(self->mpl_id, self->last_mail);
    self->last_mail = NULL;
  }
}

inline static void* GetTemplate(SimpleInbox self, GetDelegate get) {
  DeleteLastMailIfNeeded(self);
  return get(self) ? ExtractMessage(self->last_mail) : NULL;
}

static bool GetNextMail(SimpleInbox self) {
  if (tk_rcv_mbx(self->mbx_id, (T_MSG**)&self->last_mail, TMO_POL) == E_OK) {
    return true;
  } else {
    self->last_mail = NULL;  // Just in case.
    return false;
  }
}

static void* Get(Inbox self) { return GetTemplate((SimpleInbox)self, GetNextMail); }

static bool BlockingGetNextMail(SimpleInbox self) {
  if (tk_rcv_mbx(self->mbx_id, (T_MSG**)&self->last_mail, TMO_FEVR) == E_OK) {
    return true;
  } else {
    self->last_mail = NULL;  // Just in case.
    return false;
  }
}

static void* BlockingGet(Inbox self) { return GetTemplate((SimpleInbox)self, BlockingGetNextMail); }

inline static void CreateMailbox(SimpleInbox self) {
  T_CMBX mbx_packet = {.mbxatr = (TA_TFIFO | TA_MFIFO)};
  self->mbx_id = tk_cre_mbx(&mbx_packet);
}

inline static int LimitCapacity(int capacity) { return capacity > kMaxInboxCapacity ? kMaxInboxCapacity : capacity; }

inline static void CreateMemoryPool(SimpleInbox self, int capacity) {
  T_CMPL mpl_packet = {.mplatr = (TA_TFIFO | TA_RNG0), .mplsz = LimitCapacity(capacity)};
  self->mpl_id = tk_cre_mpl(&mpl_packet);
}

static Inbox New(int capacity) {
  SimpleInbox self = (SimpleInbox)heap->New(sizeof(SimpleInboxStruct));
  self->impl.Delete = Delete;
  self->impl.Post = Post;
  self->impl.BlockingPost = BlockingPost;
  self->impl.Get = Get;
  self->impl.BlockingGet = BlockingGet;
  CreateMailbox(self);
  CreateMemoryPool(self, capacity);
  return (Inbox)self;
}

static const SimpleInboxMethodStruct kTheMethod = {
    .New = New,
};

const SimpleInboxMethod simpleInbox = &kTheMethod;
