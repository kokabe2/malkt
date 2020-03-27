// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "time_unit.h"
#include "uptime.h"
}

class TimeUnitTest : public ::testing::Test {
 protected:
  virtual void SetUp() { uptime->Set(12345678); }

  virtual void TearDown() {}
};

TEST_F(TimeUnitTest, NowInMillisecond) { EXPECT_EQ(12345678, timeUnit->Now(timeUnit->Millisecond)); }

TEST_F(TimeUnitTest, NowInSecond) { EXPECT_EQ(12345, timeUnit->Now(timeUnit->Second)); }

TEST_F(TimeUnitTest, NowInMinute) { EXPECT_EQ(205, timeUnit->Now(timeUnit->Minute)); }

TEST_F(TimeUnitTest, NowInHour) { EXPECT_EQ(3, timeUnit->Now(timeUnit->Hour)); }
