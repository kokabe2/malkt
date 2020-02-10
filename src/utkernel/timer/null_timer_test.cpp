// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "../util/system_call_logger.h"
#include "null_timer.h"
}

class NullTimerTest : public ::testing::Test {
 protected:
  Timer t = kNullTimer;
  virtual void SetUp() { systemCallLogger->Reset(); }
  virtual void TearDown() {}
};

TEST_F(NullTimerTest, NullObject) {
  timer->Resume(t);
  timer->Delete(&t);

  EXPECT_STREQ("", systemCallLogger->Get());
}

TEST_F(NullTimerTest, Pause) {
  timer->Pause(t);

  EXPECT_STREQ(
      "+ tk_stp_cyc (-1)\n"
      "- tk_stp_cyc (0)\n",  // Return code is E_ID in production code.
      systemCallLogger->Get());
}
