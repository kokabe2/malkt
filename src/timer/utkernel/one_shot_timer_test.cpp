// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "../../util/system_call_logger.h"
#include "one_shot_timer.h"
#include "utkernel_cyc_spy.h"
}

namespace {
int timer_called_count;

void TimerSpy(void) { ++timer_called_count; }
}  // namespace

class OneShotTimerTest : public ::testing::Test {
 protected:
  Timer t;

  virtual void SetUp() {
    timer_called_count = 0;
    utkernelCycSpy->Reset();
    systemCallLogger->Reset();
  }

  virtual void TearDown() {
    if (t != NULL) timer->Delete(&t);
  }

  void NewOneShotTimer() {
    t = oneShotTimer->New(TimerSpy, 10);
    systemCallLogger->Reset();
  }
};

TEST_F(OneShotTimerTest, New) {
  t = oneShotTimer->New(TimerSpy, 10);

  EXPECT_TRUE(t != NULL);
  EXPECT_EQ((TA_HLNG | TA_STA | TA_PHS), utkernelCycSpy->Attribute());
  EXPECT_EQ(~0, utkernelCycSpy->CycleTime());
  EXPECT_EQ(10, utkernelCycSpy->CyclePhase());
  EXPECT_STREQ(
      "+ tk_cre_cyc\n"
      "- tk_cre_cyc (0)\n",
      systemCallLogger->Get());

  utkernelCycSpy->RunTimer();
  EXPECT_EQ(1, timer_called_count);
}

TEST_F(OneShotTimerTest, Delete) {
  NewOneShotTimer();

  timer->Delete(&t);

  EXPECT_EQ(NULL, t);
  EXPECT_STREQ(
      "+ tk_del_cyc (0)\n"
      "- tk_del_cyc (0)\n",
      systemCallLogger->Get());
}

TEST_F(OneShotTimerTest, ResumeWhenTimerIsNotDone) {
  NewOneShotTimer();

  timer->Resume(t);

  EXPECT_STREQ(
      "+ tk_sta_cyc (0)\n"
      "- tk_sta_cyc (0)\n",
      systemCallLogger->Get());
}

TEST_F(OneShotTimerTest, ResumeWhenTimerIsDone) {
  NewOneShotTimer();
  utkernelCycSpy->RunTimer();

  timer->Resume(t);

  EXPECT_STREQ("", systemCallLogger->Get());
}

TEST_F(OneShotTimerTest, IsDone) {
  NewOneShotTimer();

  EXPECT_FALSE(oneShotTimer->IsDone(t));
  utkernelCycSpy->RunTimer();
  EXPECT_TRUE(oneShotTimer->IsDone(t));
}

TEST_F(OneShotTimerTest, RunTimerOnlyOnce) {
  NewOneShotTimer();
  utkernelCycSpy->RunTimer();

  utkernelCycSpy->RunTimer();

  EXPECT_EQ(1, timer_called_count);
}
