// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "interval_timer.h"
#include "util/system_call_logger.h"
#include "utkernel_cyc_spy.h"
}

namespace {
bool was_ran;

void TimerSpy(void) { was_ran = true; }
}  // namespace

class IntervalTimerTest : public ::testing::Test {
 protected:
  Timer timer;

  virtual void SetUp() {
    was_ran = false;
    utkernelCycSpy->Reset();
    systemCallLogger->Reset();
  }

  virtual void TearDown() {
    if (timer != NULL) timer->Delete(&timer);
  }

  void NewIntervalTimer() {
    timer = intervalTimer->New(TimerSpy, 10);
    systemCallLogger->Reset();
  }
};

TEST_F(IntervalTimerTest, New) {
  timer = intervalTimer->New(TimerSpy, 10);

  EXPECT_TRUE(timer != NULL);
  EXPECT_EQ((TA_HLNG | TA_STA | TA_PHS), utkernelCycSpy->Attribute());
  EXPECT_EQ(10, utkernelCycSpy->CycleTime());
  EXPECT_EQ(10, utkernelCycSpy->CyclePhase());
  EXPECT_STREQ(
      "+ tk_cre_cyc\n"
      "- tk_cre_cyc (0)\n",
      systemCallLogger->Get());

  EXPECT_FALSE(was_ran);
  utkernelCycSpy->RunTimer();
  EXPECT_TRUE(was_ran);
}

TEST_F(IntervalTimerTest, Delete) {
  NewIntervalTimer();

  timer->Delete(&timer);

  EXPECT_EQ(NULL, timer);
  EXPECT_STREQ(
      "+ tk_del_cyc (0)\n"
      "- tk_del_cyc (0)\n",
      systemCallLogger->Get());
}

TEST_F(IntervalTimerTest, Pause) {
  NewIntervalTimer();

  timer->Pause(timer);

  EXPECT_STREQ(
      "+ tk_stp_cyc (0)\n"
      "- tk_stp_cyc (0)\n",
      systemCallLogger->Get());
}

TEST_F(IntervalTimerTest, Resume) {
  NewIntervalTimer();

  timer->Resume(timer);

  EXPECT_STREQ(
      "+ tk_sta_cyc (0)\n"
      "- tk_sta_cyc (0)\n",
      systemCallLogger->Get());
}
