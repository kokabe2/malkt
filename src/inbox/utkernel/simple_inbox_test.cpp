// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "simple_inbox.h"
#include "util/system_call_logger.h"
#include "utkernel_mbx_spy.h"
#include "utkernel_mpl_spy.h"
}

namespace {
char kDummyMessage[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
}  // namespace

class SimpleInboxTest : public ::testing::Test {
 protected:
  Inbox inbox;

  virtual void SetUp() {
    inbox = NULL;
    utkernelMbxSpy->Reset();
    utkernelMplSpy->Reset();
    systemCallLogger->Reset();
  }

  virtual void TearDown() {
    if (inbox != NULL) inbox->Delete(&inbox);
  }

  void NewSimpleInbox() {
    inbox = simpleInbox->New(1024);
    systemCallLogger->Reset();
  }
};

TEST_F(SimpleInboxTest, New) {
  inbox = simpleInbox->New(1024);

  EXPECT_TRUE(inbox != NULL);
  EXPECT_EQ((TA_TFIFO | TA_MFIFO), utkernelMbxSpy->Attribute());
  EXPECT_EQ((TA_TFIFO | TA_RNG0), utkernelMplSpy->Attribute());
  EXPECT_EQ(1024, utkernelMplSpy->Capacity());
  EXPECT_STREQ(
      "+ tk_cre_mbx\n"
      "- tk_cre_mbx (0)\n"
      "+ tk_cre_mpl\n"
      "- tk_cre_mpl (0)\n",
      systemCallLogger->Get());
}

TEST_F(SimpleInboxTest, NewWithOutRangeOfCapacity) {
  inbox = simpleInbox->New(kMaxInboxCapacity + 1);

  EXPECT_EQ(kMaxInboxCapacity, utkernelMplSpy->Capacity());
}

TEST_F(SimpleInboxTest, Delete) {
  NewSimpleInbox();

  inbox->Delete(&inbox);

  EXPECT_EQ(NULL, inbox);
  EXPECT_STREQ(
      "+ tk_del_mbx (0)\n"
      "- tk_del_mbx (0)\n"
      "+ tk_del_mpl (0)\n"
      "- tk_del_mpl (0)\n",
      systemCallLogger->Get());
}

TEST_F(SimpleInboxTest, Post) {
  NewSimpleInbox();

  EXPECT_TRUE(inbox->Post(inbox, kDummyMessage, sizeof(kDummyMessage)));
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

TEST_F(SimpleInboxTest, PostWhenMemoryBlockAcquisitionFailed) {
  NewSimpleInbox();
  utkernelMplSpy->SetReturnCode(0, -50);

  EXPECT_FALSE(inbox->Post(inbox, kDummyMessage, sizeof(kDummyMessage)));
  EXPECT_STREQ(
      "+ tk_get_mpl (0)\n"
      "- tk_get_mpl (-50)\n",
      systemCallLogger->Get());
}

TEST_F(SimpleInboxTest, BlockingPost) {
  NewSimpleInbox();

  EXPECT_TRUE(inbox->BlockingPost(inbox, kDummyMessage, sizeof(kDummyMessage)));
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

TEST_F(SimpleInboxTest, Get) {
  NewSimpleInbox();
  inbox->Post(inbox, kDummyMessage, sizeof(kDummyMessage));
  systemCallLogger->Reset();

  EXPECT_EQ(0, memcmp(kDummyMessage, inbox->Get(inbox), sizeof(kDummyMessage)));
  EXPECT_EQ(TMO_POL, utkernelMbxSpy->Timeout());
  EXPECT_STREQ(
      "+ tk_rcv_mbx (0)\n"
      "- tk_rcv_mbx (0)\n",
      systemCallLogger->Get());
}

TEST_F(SimpleInboxTest, GetWhenNoMessageOrSomethingFailed) {
  NewSimpleInbox();
  inbox->Post(inbox, kDummyMessage, sizeof(kDummyMessage));
  utkernelMbxSpy->SetReturnCode(0, -50);
  systemCallLogger->Reset();

  EXPECT_EQ(NULL, inbox->Get(inbox));
  EXPECT_STREQ(
      "+ tk_rcv_mbx (0)\n"
      "- tk_rcv_mbx (-50)\n",
      systemCallLogger->Get());
}

TEST_F(SimpleInboxTest, GetAfterTheSecondTime) {
  NewSimpleInbox();
  inbox->Post(inbox, kDummyMessage, sizeof(kDummyMessage));
  inbox->Get(inbox);
  inbox->Post(inbox, kDummyMessage, sizeof(kDummyMessage));
  systemCallLogger->Reset();

  inbox->Get(inbox);

  EXPECT_STREQ(
      "+ tk_rel_mpl (0)\n"
      "- tk_rel_mpl (0)\n"
      "+ tk_rcv_mbx (0)\n"
      "- tk_rcv_mbx (0)\n",
      systemCallLogger->Get());
}

TEST_F(SimpleInboxTest, BlockingGet) {
  NewSimpleInbox();
  inbox->Post(inbox, kDummyMessage, sizeof(kDummyMessage));
  systemCallLogger->Reset();

  EXPECT_EQ(0, memcmp(kDummyMessage, inbox->BlockingGet(inbox), sizeof(kDummyMessage)));
  EXPECT_EQ(TMO_FEVR, utkernelMbxSpy->Timeout());
  EXPECT_STREQ(
      "+ tk_rcv_mbx (0)\n"
      "- tk_rcv_mbx (0)\n",
      systemCallLogger->Get());
}
