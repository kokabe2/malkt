// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "../../util/system_call_logger.h"
#include "interval_timer.h"
#include "utkernel_cyc_spy.h"
}

namespace {
bool was_ran;

void TimerSpy(void) { was_ran = true; }
}  // namespace

class IntervalTimerTest : public ::testing::Test {
 protected:
  Timer t;

  virtual void SetUp() {
    was_ran = false;
    utkernelCycSpy->Reset();
    systemCallLogger->Reset();
  }

  virtual void TearDown() {
    if (t != NULL) timer->Delete(&t);
  }

  void NewIntervalTimer() {
    t = intervalTimer->New(TimerSpy, 10);
    systemCallLogger->Reset();
  }
};

TEST_F(IntervalTimerTest, New) {
  t = intervalTimer->New(TimerSpy, 10);

  EXPECT_TRUE(t != NULL);
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

  timer->Delete(&t);

  EXPECT_EQ(NULL, t);
  EXPECT_STREQ(
      "+ tk_del_cyc (0)\n"
      "- tk_del_cyc (0)\n",
      systemCallLogger->Get());
}

TEST_F(IntervalTimerTest, Pause) {
  NewIntervalTimer();

  timer->Pause(t);

  EXPECT_STREQ(
      "+ tk_stp_cyc (0)\n"
      "- tk_stp_cyc (0)\n",
      systemCallLogger->Get());
}

TEST_F(IntervalTimerTest, Resume) {
  NewIntervalTimer();

  timer->Resume(t);

  EXPECT_STREQ(
      "+ tk_sta_cyc (0)\n"
      "- tk_sta_cyc (0)\n",
      systemCallLogger->Get());
}
