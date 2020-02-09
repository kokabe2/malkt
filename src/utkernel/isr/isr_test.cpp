// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "../util/system_call_logger.h"
#include "isr.h"
#include "utkernel_int_spy.h"
}

namespace {
void InterruptDummy(int intno) {}
}  // namespace

class IsrTest : public ::testing::Test {
 protected:
  Isr i;
  virtual void SetUp() {
    utkernelIntSpy->Reset();
    i = isr->New(24, InterruptDummy);
    systemCallLogger->Reset();
  }
  virtual void TearDown() { isr->Delete(&i); }
};

TEST_F(IsrTest, New) {
  Isr instance = isr->New(24, InterruptDummy);

  EXPECT_TRUE(i != NULL);
  EXPECT_EQ((FP)InterruptDummy, utkernelIntSpy->InterruptHandler());
  EXPECT_EQ(TA_HLNG, utkernelIntSpy->Attribute());
  EXPECT_STREQ(
      "+ tk_def_int (24)\n"
      "- tk_def_int (0)\n",
      systemCallLogger->Get());

  isr->Delete(&instance);
}

TEST_F(IsrTest, NewWhenInterruptDefinitionFailed) {
  utkernelIntSpy->SetReturnCode(0, -34);

  EXPECT_EQ(NULL, isr->New(24, InterruptDummy));
  EXPECT_STREQ(
      "+ tk_def_int (24)\n"
      "- tk_def_int (-34)\n",
      systemCallLogger->Get());
}

TEST_F(IsrTest, NewWithInvelidArgument) {
  EXPECT_EQ(NULL, isr->New(-1, InterruptDummy));
  EXPECT_EQ(NULL, isr->New(24, NULL));
  EXPECT_STREQ("", systemCallLogger->Get());
}

TEST_F(IsrTest, Delete) {
  isr->Delete(&i);

  EXPECT_EQ(NULL, i);
  EXPECT_NE((FP)InterruptDummy, utkernelIntSpy->InterruptHandler());
  EXPECT_STREQ(
      "+ DisableInt (24)\n"
      "- DisableInt\n"
      "+ tk_def_int (24)\n"
      "- tk_def_int (0)\n",
      systemCallLogger->Get());
}

TEST_F(IsrTest, DeleteMultipleTimes) {
  isr->Delete(&i);
  systemCallLogger->Reset();

  isr->Delete(&i);

  EXPECT_STREQ("", systemCallLogger->Get());
}

TEST_F(IsrTest, Enable) {
  isr->Enable(i, 4);

  EXPECT_EQ(4, utkernelIntSpy->Level());
  EXPECT_STREQ(
      "+ EnableInt (24)\n"
      "- EnableInt\n",
      systemCallLogger->Get());
}

TEST_F(IsrTest, EnableWithInvalidArgument) {
  isr->Enable(i, -1);

  EXPECT_STREQ("", systemCallLogger->Get());
}

TEST_F(IsrTest, Disable) {
  isr->Enable(i, 4);
  systemCallLogger->Reset();

  isr->Disable(i);

  EXPECT_STREQ(
      "+ DisableInt (24)\n"
      "- DisableInt\n",
      systemCallLogger->Get());
}

TEST_F(IsrTest, DestroyWithNull) {
  isr->Delete(NULL);
  isr->Enable(NULL, 4);
  isr->Disable(NULL);

  SUCCEED();
}
