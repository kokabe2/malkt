// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "one_shot_timer.h"
#include "util/system_call_logger.h"
#include "utkernel_cyc_spy.h"
}

namespace {
int timer_called_count;

void TimerSpy(void) { ++timer_called_count; }
}  // namespace

class OneShotTimerTest : public ::testing::Test {
 protected:
  Timer timer;

  virtual void SetUp() {
    timer_called_count = 0;
    utkernelCycSpy->Reset();
    systemCallLogger->Reset();
  }

  virtual void TearDown() {
    if (timer != NULL) timer->Delete(&timer);
  }

  void NewOneShotTimer() {
    timer = oneShotTimer->New(TimerSpy, 10);
    systemCallLogger->Reset();
  }
};

TEST_F(OneShotTimerTest, New) {
  timer = oneShotTimer->New(TimerSpy, 10);

  EXPECT_TRUE(timer != NULL);
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

  timer->Delete(&timer);

  EXPECT_EQ(NULL, timer);
  EXPECT_STREQ(
      "+ tk_del_cyc (0)\n"
      "- tk_del_cyc (0)\n",
      systemCallLogger->Get());
}

TEST_F(OneShotTimerTest, ResumeWhenTimerIsNotDone) {
  NewOneShotTimer();

  timer->Resume(timer);

  EXPECT_STREQ(
      "+ tk_sta_cyc (0)\n"
      "- tk_sta_cyc (0)\n",
      systemCallLogger->Get());
}

TEST_F(OneShotTimerTest, ResumeWhenTimerIsDone) {
  NewOneShotTimer();
  utkernelCycSpy->RunTimer();

  timer->Resume(timer);

  EXPECT_STREQ("", systemCallLogger->Get());
}

TEST_F(OneShotTimerTest, IsDone) {
  NewOneShotTimer();

  EXPECT_FALSE(oneShotTimer->IsDone(timer));
  utkernelCycSpy->RunTimer();
  EXPECT_TRUE(oneShotTimer->IsDone(timer));
}

TEST_F(OneShotTimerTest, RunTimerOnlyOnce) {
  NewOneShotTimer();
  utkernelCycSpy->RunTimer();

  utkernelCycSpy->RunTimer();

  EXPECT_EQ(1, timer_called_count);
}
