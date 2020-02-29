// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "../util/system_call_logger.h"
#include "one_shot_timer.h"
#include "timer_handler_spy.h"
#include "utkernel_cyc_spy.h"
}

class OneShotTimerTest : public ::testing::Test {
 protected:
  Timer t;

  virtual void SetUp() {
    timerHandlerSpy->Reset();
    utkernelCycSpy->Reset();
    t = oneShotTimer->New(timerHandlerSpy->Get(), 10);
    systemCallLogger->Reset();
  }

  virtual void TearDown() {
    if (t != NULL) timer->Delete(&t);
  }
};

TEST_F(OneShotTimerTest, New) {
  Timer instance = oneShotTimer->New(timerHandlerSpy->Get(), 10);

  EXPECT_TRUE(instance != NULL);
  EXPECT_EQ((TA_HLNG | TA_STA | TA_PHS), utkernelCycSpy->Attribute());
  EXPECT_EQ(~0, utkernelCycSpy->CycleTime());
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

TEST_F(OneShotTimerTest, NewWhenTimerCreationFailed) {
  utkernelCycSpy->SetReturnCode(0, -34);

  EXPECT_EQ(NULL, oneShotTimer->New(timerHandlerSpy->Get(), 10));
  EXPECT_STREQ(
      "+ tk_cre_cyc\n"
      "- tk_cre_cyc (-34)\n",
      systemCallLogger->Get());
}

TEST_F(OneShotTimerTest, NewWithInvalidArgument) {
  EXPECT_EQ(NULL, oneShotTimer->New(NULL, 10));
  EXPECT_EQ(NULL, oneShotTimer->New(timerHandlerSpy->Get(), 0));
  EXPECT_EQ(NULL, oneShotTimer->New(timerHandlerSpy->Get(), -128));
  EXPECT_STREQ("", systemCallLogger->Get());
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
  timerHandlerSpy->Reset();

  utkernelCycSpy->RunTimer();

  EXPECT_FALSE(timerHandlerSpy->WasRun());
}
