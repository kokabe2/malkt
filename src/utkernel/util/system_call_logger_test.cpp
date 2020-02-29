// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "system_call_logger.h"
}

class SystemCallLoggerTest : public ::testing::Test {
 protected:
  virtual void SetUp() { systemCallLogger->Reset(); }
  virtual void TearDown() {}
};

TEST_F(SystemCallLoggerTest, ConditionAfterNew) { EXPECT_STREQ("", systemCallLogger->Get()); }

TEST_F(SystemCallLoggerTest, LogSystemCallOnce) {
  systemCallLogger->Enter("test", 0);
  systemCallLogger->Exit("test", -1);

  EXPECT_STREQ(
      "+ test (0)\n"
      "- test (-1)\n",
      systemCallLogger->Get());
}

TEST_F(SystemCallLoggerTest, LogSystemCallMultipleTimes) {
  systemCallLogger->Enter("test", 6);
  systemCallLogger->Enter("hello", 128);
  systemCallLogger->Enter("world", kNonsenseParameter);
  systemCallLogger->Exit("world", 0);
  systemCallLogger->Exit("hello", kNonsenseParameter);
  systemCallLogger->Exit("test", 39);

  EXPECT_STREQ(
      "+ test (6)\n"
      "++ hello (128)\n"
      "+++ world\n"
      "--- world (0)\n"
      "-- hello\n"
      "- test (39)\n",
      systemCallLogger->Get());
}
