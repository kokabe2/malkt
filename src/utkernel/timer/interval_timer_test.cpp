// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "../util/system_call_logger.h"
#include "interval_timer.h"
#include "timer_handler_spy.h"
#include "utkernel_cyc_spy.h"
}

class IntervalTimerTest : public ::testing::Test {
 protected:
  Timer t;

  virtual void SetUp() {
    timerHandlerSpy->Reset();
    utkernelCycSpy->Reset();
    t = intervalTimer->New(timerHandlerSpy->Get(), 10);
    systemCallLogger->Reset();
  }

  virtual void TearDown() {
    if (t != NULL) timer->Delete(&t);
  }
};

TEST_F(IntervalTimerTest, New) {
  Timer instance = intervalTimer->New(timerHandlerSpy->Get(), 10);

  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ((TA_HLNG | TA_STA | TA_PHS), utkernelCycSpy->Attribute());
  EXPECT_EQ(10, utkernelCycSpy->CycleTime());
  EXPECT_EQ(10, utkernelCycSpy->CyclePhase());
  EXPECT_STREQ(
      "+ tk_cre_cyc\n"
      "- tk_cre_cyc (0)\n",
      systemCallLogger->Get());

  EXPECT_FALSE(timerHandlerSpy->WasRun());
  utkernelCycSpy->RunTimer();
  EXPECT_TRUE(timerHandlerSpy->WasRun());

  timer->Delete(&instance);
}

TEST_F(IntervalTimerTest, NewWhenTimerCreationFailed) {
  utkernelCycSpy->SetReturnCode(0, -34);

  EXPECT_EQ(NULL, intervalTimer->New(timerHandlerSpy->Get(), 10));
  EXPECT_STREQ(
      "+ tk_cre_cyc\n"
      "- tk_cre_cyc (-34)\n",
      systemCallLogger->Get());
}

TEST_F(IntervalTimerTest, Delete) {
  timer->Delete(&t);

  EXPECT_EQ(NULL, t);
  EXPECT_STREQ(
      "+ tk_del_cyc (0)\n"
      "- tk_del_cyc (0)\n",
      systemCallLogger->Get());
}

TEST_F(IntervalTimerTest, Pause) {
  timer->Pause(t);

  EXPECT_STREQ(
      "+ tk_stp_cyc (0)\n"
      "- tk_stp_cyc (0)\n",
      systemCallLogger->Get());
}

TEST_F(IntervalTimerTest, Resume) {
  timer->Resume(t);

  EXPECT_STREQ(
      "+ tk_sta_cyc (0)\n"
      "- tk_sta_cyc (0)\n",
      systemCallLogger->Get());
}
