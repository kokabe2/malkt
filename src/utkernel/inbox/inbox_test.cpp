// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "../util/system_call_logger.h"
#include "inbox.h"
#include "utkernel_mbx_spy.h"
#include "utkernel_mpl_spy.h"
}

namespace {
char kDummyMessage[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
}  // namespace

class InboxTest : public ::testing::Test {
 protected:
  Inbox i;

  virtual void SetUp() {
    utkernelMbxSpy->Reset();
    utkernelMplSpy->Reset();
    i = inbox->New(1024);
    systemCallLogger->Reset();
  }

  virtual void TearDown() {
    if (i != NULL) inbox->Delete(&i);
  }
};

TEST_F(InboxTest, New) {
  Inbox instance = inbox->New(1024);

  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ((TA_TFIFO | TA_MFIFO), utkernelMbxSpy->Attribute());
  EXPECT_EQ((TA_TFIFO | TA_RNG0), utkernelMplSpy->Attribute());
  EXPECT_EQ(1024, utkernelMplSpy->Capacity());
  EXPECT_STREQ(
      "+ tk_cre_mbx\n"
      "- tk_cre_mbx (0)\n"
      "+ tk_cre_mpl\n"
      "- tk_cre_mpl (0)\n",
      systemCallLogger->Get());

  inbox->Delete(&instance);
}

TEST_F(InboxTest, NewWithOutRangeOfCapacity) {
  Inbox instance = inbox->New(kMaxInboxCapacity + 1);

  EXPECT_EQ(kMaxInboxCapacity, utkernelMplSpy->Capacity());

  inbox->Delete(&instance);
}

TEST_F(InboxTest, Delete) {
  inbox->Delete(&i);

  EXPECT_EQ(NULL, i);
  EXPECT_STREQ(
      "+ tk_del_mbx (0)\n"
      "- tk_del_mbx (0)\n"
      "+ tk_del_mpl (0)\n"
      "- tk_del_mpl (0)\n",
      systemCallLogger->Get());
}

TEST_F(InboxTest, Post) {
  EXPECT_TRUE(inbox->Post(i, kDummyMessage, sizeof(kDummyMessage)));
  EXPECT_EQ(0, memcmp(kDummyMessage, utkernelMbxSpy->LastMessage(), sizeof(kDummyMessage)));
  EXPECT_EQ(sizeof(T_MSG) + sizeof(kDummyMessage), utkernelMplSpy->BlockSize());
  EXPECT_EQ(TMO_POL, utkernelMplSpy->Timout());
  EXPECT_STREQ(
      "+ tk_get_mpl (0)\n"
      "- tk_get_mpl (0)\n"
      "+ tk_snd_mbx (0)\n"
      "- tk_snd_mbx (0)\n",
      systemCallLogger->Get());
}

TEST_F(InboxTest, PostWhenMemoryBlockAcquisitionFailed) {
  utkernelMplSpy->SetReturnCode(0, -50);

  EXPECT_FALSE(inbox->Post(i, kDummyMessage, sizeof(kDummyMessage)));
  EXPECT_STREQ(
      "+ tk_get_mpl (0)\n"
      "- tk_get_mpl (-50)\n",
      systemCallLogger->Get());
}

TEST_F(InboxTest, PostWithInvalidArguments) {
  EXPECT_FALSE(inbox->Post(i, kDummyMessage, 0));
  EXPECT_FALSE(inbox->Post(i, kDummyMessage, -16));
  EXPECT_FALSE(inbox->Post(i, NULL, 16));
  EXPECT_STREQ("", systemCallLogger->Get());
}

TEST_F(InboxTest, BlockingPost) {
  EXPECT_TRUE(inbox->BlockingPost(i, kDummyMessage, sizeof(kDummyMessage)));
  EXPECT_EQ(0, memcmp(kDummyMessage, utkernelMbxSpy->LastMessage(), sizeof(kDummyMessage)));
  EXPECT_EQ(sizeof(T_MSG) + sizeof(kDummyMessage), utkernelMplSpy->BlockSize());
  EXPECT_EQ(TMO_FEVR, utkernelMplSpy->Timout());
  EXPECT_STREQ(
      "+ tk_get_mpl (0)\n"
      "- tk_get_mpl (0)\n"
      "+ tk_snd_mbx (0)\n"
      "- tk_snd_mbx (0)\n",
      systemCallLogger->Get());
}

TEST_F(InboxTest, Get) {
  inbox->Post(i, kDummyMessage, sizeof(kDummyMessage));
  systemCallLogger->Reset();

  EXPECT_EQ(0, memcmp(kDummyMessage, inbox->Get(i), sizeof(kDummyMessage)));
  EXPECT_EQ(TMO_POL, utkernelMbxSpy->Timeout());
  EXPECT_STREQ(
      "+ tk_rcv_mbx (0)\n"
      "- tk_rcv_mbx (0)\n",
      systemCallLogger->Get());
}

TEST_F(InboxTest, GetWhenNoMessageOrSomethingFailed) {
  inbox->Post(i, kDummyMessage, sizeof(kDummyMessage));
  utkernelMbxSpy->SetReturnCode(0, -50);
  systemCallLogger->Reset();

  EXPECT_EQ(NULL, inbox->Get(i));
  EXPECT_STREQ(
      "+ tk_rcv_mbx (0)\n"
      "- tk_rcv_mbx (-50)\n",
      systemCallLogger->Get());
}

TEST_F(InboxTest, GetAfterTheSecondTime) {
  inbox->Post(i, kDummyMessage, sizeof(kDummyMessage));
  inbox->Get(i);
  inbox->Post(i, kDummyMessage, sizeof(kDummyMessage));
  systemCallLogger->Reset();

  inbox->Get(i);

  EXPECT_STREQ(
      "+ tk_rel_mpl (0)\n"
      "- tk_rel_mpl (0)\n"
      "+ tk_rcv_mbx (0)\n"
      "- tk_rcv_mbx (0)\n",
      systemCallLogger->Get());
}

TEST_F(InboxTest, BlockingGet) {
  inbox->Post(i, kDummyMessage, sizeof(kDummyMessage));
  systemCallLogger->Reset();

  EXPECT_EQ(0, memcmp(kDummyMessage, inbox->BlockingGet(i), sizeof(kDummyMessage)));
  EXPECT_EQ(TMO_FEVR, utkernelMbxSpy->Timeout());
  EXPECT_STREQ(
      "+ tk_rcv_mbx (0)\n"
      "- tk_rcv_mbx (0)\n",
      systemCallLogger->Get());
}
