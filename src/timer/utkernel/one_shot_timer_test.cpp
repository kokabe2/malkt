// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "../../util/system_call_logger.h"
#include "one_shot_timer.h"
#include "utkernel_cyc_spy.h"
}

namespace {
bool was_ran;

void TimerSpy(void) { was_ran = true; }
}  // namespace

class OneShotTimerTest : public ::testing::Test {
 protected:
  Timer t;

  virtual void SetUp() {
    was_ran = false;
    utkernelCycSpy->Reset();
    t = oneShotTimer->New(TimerSpy, 10);
    systemCallLogger->Reset();
  }

  virtual void TearDown() {
    if (t != NULL) timer->Delete(&t);
  }
};

TEST_F(OneShotTimerTest, New) {
  Timer instance = oneShotTimer->New(TimerSpy, 10);

  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ((TA_HLNG | TA_STA | TA_PHS), utkernelCycSpy->Attribute());
  EXPECT_EQ(~0, utkernelCycSpy->CycleTime());
  EXPECT_EQ(10, utkernelCycSpy->CyclePhase());
  EXPECT_STREQ(
      "+ tk_cre_cyc\n"
      "- tk_cre_cyc (0)\n",
      systemCallLogger->Get());

  EXPECT_FALSE(was_ran);
  utkernelCycSpy->RunTimer();
  EXPECT_TRUE(was_ran);

  timer->Delete(&instance);
}

TEST_F(OneShotTimerTest, ResumeWhenTimerIsNotDone) {
  timer->Resume(t);

  EXPECT_STREQ(
      "+ tk_sta_cyc (0)\n"
      "- tk_sta_cyc (0)\n",
      systemCallLogger->Get());
}

TEST_F(OneShotTimerTest, ResumeWhenTimerIsDone) {
  utkernelCycSpy->RunTimer();

  timer->Resume(t);

  EXPECT_STREQ("", systemCallLogger->Get());
}

TEST_F(OneShotTimerTest, IsDone) {
  EXPECT_FALSE(oneShotTimer->IsDone(t));
  utkernelCycSpy->RunTimer();
  EXPECT_TRUE(oneShotTimer->IsDone(t));
}

TEST_F(OneShotTimerTest, RunTimerOnlyOnce) {
  utkernelCycSpy->RunTimer();
  was_ran = false;

  utkernelCycSpy->RunTimer();

  EXPECT_FALSE(was_ran);
}
