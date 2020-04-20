// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "basic_isr.h"
#include "util/system_call_logger.h"
#include "utkernel_int_spy.h"
}

namespace {
void DummyInterrupt(int unused) {}
}  // namespace

class BasicIsrTest : public ::testing::Test {
 protected:
  Isr isr;

  virtual void SetUp() {
    utkernelIntSpy->Reset();
    systemCallLogger->Reset();
  }

  virtual void TearDown() {
    if (isr != NULL) isr->Delete(&isr);
  }

  void NewBasicIsr(void) {
    isr = basicIsr->New(24, 8, DummyInterrupt);
    systemCallLogger->Reset();
  }
};

TEST_F(BasicIsrTest, New) {
  isr = basicIsr->New(24, 8, DummyInterrupt);

  ASSERT_TRUE(isr != NULL);
  EXPECT_EQ((FP)DummyInterrupt, utkernelIntSpy->InterruptHandler());
  EXPECT_EQ(TA_HLNG, utkernelIntSpy->Attribute());
  EXPECT_STREQ(
      "+ tk_def_int (24)\n"
      "- tk_def_int (0)\n",
      systemCallLogger->Get());
}

TEST_F(BasicIsrTest, Delete) {
  NewBasicIsr();

  isr->Delete(&isr);

  EXPECT_EQ(NULL, isr);
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

  isr->Enable(isr);

  EXPECT_EQ(8, utkernelIntSpy->Level());
  EXPECT_STREQ(
      "+ EnableInt (24)\n"
      "- EnableInt\n",
      systemCallLogger->Get());
}

TEST_F(BasicIsrTest, Disable) {
  NewBasicIsr();

  isr->Disable(isr);

  EXPECT_STREQ(
      "+ DisableInt (24)\n"
      "- DisableInt\n",
      systemCallLogger->Get());
}
