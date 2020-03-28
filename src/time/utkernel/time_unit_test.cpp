// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "../../task/utkernel/utkernel_tsk_spy.h"
#include "../../util/system_call_logger.h"
#include "time_unit.h"
#include "uptime.h"
}

class TimeUnitTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    utkernelTskSpy->Reset();
    systemCallLogger->Reset();
    uptime->Set(12345678);
  }

  virtual void TearDown() {}
};

TEST_F(TimeUnitTest, NowInMillisecond) { EXPECT_EQ(12345678, timeUnit->Now(timeUnit->Millisecond)); }

TEST_F(TimeUnitTest, NowInSecond) { EXPECT_EQ(12345, timeUnit->Now(timeUnit->Second)); }

TEST_F(TimeUnitTest, NowInMinute) { EXPECT_EQ(205, timeUnit->Now(timeUnit->Minute)); }

TEST_F(TimeUnitTest, NowInHour) { EXPECT_EQ(3, timeUnit->Now(timeUnit->Hour)); }

TEST_F(TimeUnitTest, SleepInMillisecond) {
  timeUnit->Sleep(20 * timeUnit->Millisecond);

  EXPECT_EQ(20, utkernelTskSpy->DelayTime());
  EXPECT_STREQ(
      "+ tk_dly_tsk\n"
      "- tk_dly_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(TimeUnitTest, SleepInSecond) {
  timeUnit->Sleep(20 * timeUnit->Second);

  EXPECT_EQ(20000, utkernelTskSpy->DelayTime());
  EXPECT_STREQ(
      "+ tk_dly_tsk\n"
      "- tk_dly_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(TimeUnitTest, SleepInMinute) {
  timeUnit->Sleep(20 * timeUnit->Minute);

  EXPECT_EQ(1200000, utkernelTskSpy->DelayTime());
  EXPECT_STREQ(
      "+ tk_dly_tsk\n"
      "- tk_dly_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(TimeUnitTest, SleepInHour) {
  timeUnit->Sleep(20 * timeUnit->Hour);

  EXPECT_EQ(72000000, utkernelTskSpy->DelayTime());
  EXPECT_STREQ(
      "+ tk_dly_tsk\n"
      "- tk_dly_tsk (0)\n",
      systemCallLogger->Get());
}
