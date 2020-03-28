// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "../../util/system_call_logger.h"
#include "basic_isr.h"
#include "utkernel_int_spy.h"
}

namespace {
void DummyInterrupt(int unused) {}
}  // namespace

class BasicIsrTest : public ::testing::Test {
 protected:
  Isr i;

  virtual void SetUp() {
    utkernelIntSpy->Reset();
    systemCallLogger->Reset();
  }

  virtual void TearDown() {
    if (i != NULL) isr->Delete(&i);
  }

  void NewBasicIsr(void) {
    i = basicIsr->New(24, 8, DummyInterrupt);
    systemCallLogger->Reset();
  }
};

TEST_F(BasicIsrTest, New) {
  i = basicIsr->New(24, 8, DummyInterrupt);

  ASSERT_TRUE(i != NULL);
  EXPECT_EQ((FP)DummyInterrupt, utkernelIntSpy->InterruptHandler());
  EXPECT_EQ(TA_HLNG, utkernelIntSpy->Attribute());
  EXPECT_STREQ(
      "+ tk_def_int (24)\n"
      "- tk_def_int (0)\n",
      systemCallLogger->Get());
}

TEST_F(BasicIsrTest, Delete) {
  NewBasicIsr();

  isr->Delete(&i);

  EXPECT_EQ(NULL, i);
  EXPECT_NE((FP)DummyInterrupt, utkernelIntSpy->InterruptHandler());
  EXPECT_STREQ(
      "+ DisableInt (24)\n"
      "- DisableInt\n"
      "+ tk_def_int (24)\n"
      "- tk_def_int (0)\n",
      systemCallLogger->Get());
}

TEST_F(BasicIsrTest, Enable) {
  NewBasicIsr();

  isr->Enable(i);

  EXPECT_EQ(8, utkernelIntSpy->Level());
  EXPECT_STREQ(
      "+ EnableInt (24)\n"
      "- EnableInt\n",
      systemCallLogger->Get());
}

TEST_F(BasicIsrTest, Disable) {
  NewBasicIsr();

  isr->Disable(i);

  EXPECT_STREQ(
      "+ DisableInt (24)\n"
      "- DisableInt\n",
      systemCallLogger->Get());
}
