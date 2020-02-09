// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "inbox.h"

#include <stdint.h>
#include <string.h>

#include "bleu/v1/heap.h"
#include "utkernel/utkernel.h"

typedef void* (*ComposeDelegate)(Inbox self, const void* message, int size);
typedef bool (*GetDelegate)(Inbox self);
typedef struct InboxStruct {
  int mbx_id;
  int mpl_id;
  void* last_mail;
} InboxStruct;

static const int kHeaderSize = sizeof(T_MSG);

inline static bool IsValid(int capacity) {
  return capacity > 0 && capacity <= kMaxInboxCapacity;
}
inline static bool CreateInbox(Inbox self, int capacity) {
  T_CMBX mbx_packet = {.mbxatr = (TA_TFIFO | TA_MFIFO)};
  if ((self->mbx_id = tk_cre_mbx(&mbx_packet)) < 0) return false;

  T_CMPL mpl_packet = {.mplatr = (TA_TFIFO | TA_RNG0), .mplsz = capacity};
  if ((self->mpl_id = tk_cre_mpl(&mpl_packet)) >= 0) return true;

  tk_del_mbx(self->mbx_id);
  return false;
}
static Inbox New(int capacity) {
  if (!IsValid(capacity)) return NULL;
  Inbox self = (Inbox)heap->New(sizeof(InboxStruct));
  if (self && !CreateInbox(self, capacity)) heap->Delete((void**)&self);
  return self;
}
static void Delete(Inbox* self) {
  if (!self || !(*self)) return;
  tk_del_mbx((*self)->mbx_id);
  tk_del_mpl((*self)->mpl_id);
  heap->Delete((void**)self);
}
inline static bool Validate(const void* message, int size) {
  return message && size > 0;
}
inline static void* GetMemoryBlock(Inbox self, int size, TMO timeout) {
  void* mail;
  return tk_get_mpl(self->mpl_id, kHeaderSize + size, &mail, timeout) == E_OK
             ? mail
             : NULL;
}
inline static void* ExtractMessage(void* mail) {
  return (void*)((uintptr_t)mail + kHeaderSize);
}
inline static void EditMail(void* mail, const void* message, int size) {
  memcpy(ExtractMessage(mail), message, size);
}
inline static void SendMail(Inbox self, void* mail) {
  tk_snd_mbx(self->mbx_id, (T_MSG*)mail);
}
inline static bool PostTemplate(Inbox self, const void* message, int size,
                                ComposeDelegate compose) {
  if (!self || !Validate(message, size)) return false;
  void* mail = compose(self, message, size);
  if (mail) SendMail(self, mail);
  return mail;
}
static void* ComposeMail(Inbox self, const void* message, int size) {
  void* mail = GetMemoryBlock(self, size, TMO_POL);
  if (mail) EditMail(mail, message, size);
  return mail;
}
static bool Post(Inbox self, const void* message, int size) {
  return PostTemplate(self, message, size, ComposeMail);
}
static void* BlockingComposeMail(Inbox self, const void* message, int size) {
  void* mail = GetMemoryBlock(self, size, TMO_FEVR);
  if (mail) EditMail(mail, message, size);
  return mail;
}
static bool BlockingPost(Inbox self, const void* message, int size) {
  return PostTemplate(self, message, size, BlockingComposeMail);
}
inline static void DeleteLastMailIfNeeded(Inbox self) {
  if (!self->last_mail) return;
  tk_rel_mpl(self->mpl_id, self->last_mail);
  self->last_mail = NULL;
}
static bool GetNextMail(Inbox self) {
  if (tk_rcv_mbx(self->mbx_id, (T_MSG**)&self->last_mail, TMO_POL) != E_OK)
    self->last_mail = NULL;  // Just in case.
  return self->last_mail;
}
inline static void* GetTemplate(Inbox self, GetDelegate get) {
  if (!self) return NULL;
  DeleteLastMailIfNeeded(self);
  return get(self) ? ExtractMessage(self->last_mail) : NULL;
}
static void* Get(Inbox self) { return GetTemplate(self, GetNextMail); }
static bool BlockingGetNextMail(Inbox self) {
  if (tk_rcv_mbx(self->mbx_id, (T_MSG**)&self->last_mail, TMO_FEVR) != E_OK)
    self->last_mail = NULL;  // Just in case.
  return self->last_mail;
}
static void* BlockingGet(Inbox self) {
  return GetTemplate(self, BlockingGetNextMail);
}
static const InboxMethodStruct kTheMethod = {
    .New = New,
    .Delete = Delete,
    .Post = Post,
    .BlockingPost = BlockingPost,
    .Get = Get,
    .BlockingGet = BlockingGet,
};
const InboxMethod inbox = &kTheMethod;
