// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "../util/system_call_logger.h"
#include "function_entry_spy.h"
#include "task.h"
#include "utkernel_tsk_spy.h"
}

class TaskTest : public ::testing::Test {
 protected:
  Task t;

  virtual void SetUp() {
    functionEntrySpy->Reset();
    utkernelTskSpy->Reset();
    t = task->New(functionEntrySpy->Get(), 4, kMaxTaskStackSize);
    systemCallLogger->Reset();
  }

  virtual void TearDown() {
    if (t != NULL) task->Delete(&t);
  }
};

TEST_F(TaskTest, New) {
  Task instance = task->New(functionEntrySpy->Get(), 4, kMaxTaskStackSize);

  ASSERT_TRUE(instance != NULL);
  EXPECT_EQ(TA_HLNG | TA_RNG0, utkernelTskSpy->Attribute());
  EXPECT_EQ(9, utkernelTskSpy->Priority());
  EXPECT_EQ(kMaxTaskStackSize, utkernelTskSpy->StackSize());
  EXPECT_FALSE(functionEntrySpy->WasRun());
  EXPECT_STREQ(
      "+ tk_cre_tsk\n"
      "- tk_cre_tsk (0)\n",
      systemCallLogger->Get());

  task->Delete(&instance);
}

TEST_F(TaskTest, NewWithBoundaryPriority) {
  Task highest_priority = task->New(functionEntrySpy->Get(), kHighestTaskPriority, kMaxTaskStackSize);
  EXPECT_EQ(5, utkernelTskSpy->Priority());
  task->Delete(&highest_priority);

  Task lowest_priority = task->New(functionEntrySpy->Get(), kLowestTaskPriority, kMaxTaskStackSize);
  EXPECT_EQ(12, utkernelTskSpy->Priority());
  task->Delete(&lowest_priority);
}

TEST_F(TaskTest, NewWhenTaskCreationFailed) {
  utkernelTskSpy->SetReturnCode(0, -34);

  EXPECT_EQ(NULL, task->New(functionEntrySpy->Get(), 4, kMaxTaskStackSize));
  EXPECT_STREQ(
      "+ tk_cre_tsk\n"
      "- tk_cre_tsk (-34)\n",
      systemCallLogger->Get());
}

TEST_F(TaskTest, NewWithInvalidArguments) {
  EXPECT_EQ(NULL, task->New(NULL, 4, kMaxTaskStackSize));
  EXPECT_EQ(NULL, task->New(functionEntrySpy->Get(), kLowestTaskPriority - 1, kMaxTaskStackSize));
  EXPECT_EQ(NULL, task->New(functionEntrySpy->Get(), kHighestTaskPriority + 1, kMaxTaskStackSize));
  EXPECT_EQ(NULL, task->New(functionEntrySpy->Get(), 4, 0));
  EXPECT_EQ(NULL, task->New(functionEntrySpy->Get(), 4, kMaxTaskStackSize + 1));
  EXPECT_STREQ("", systemCallLogger->Get());
}

TEST_F(TaskTest, DeleteSelfTask) {
  task->Delete(&t);

  EXPECT_EQ(NULL, t);
  EXPECT_STREQ(
      "+ tk_get_tid\n"
      "- tk_get_tid (0)\n"
      "+ tk_exd_tsk\n"
      "- tk_exd_tsk\n",
      systemCallLogger->Get());
}

TEST_F(TaskTest, DeleteOtherTask) {
  utkernelTskSpy->SetReturnCode(0, 1);

  task->Delete(&t);

  EXPECT_EQ(NULL, t);
  EXPECT_STREQ(
      "+ tk_get_tid\n"
      "- tk_get_tid (1)\n"
      "+ tk_ter_tsk (0)\n"
      "- tk_ter_tsk (0)\n"
      "+ tk_del_tsk (0)\n"
      "- tk_del_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(TaskTest, Run) {
  task->Run(t);

  EXPECT_TRUE(functionEntrySpy->WasRun());
  EXPECT_STREQ(
      "+ tk_sta_tsk (0)\n"
      "++ tk_ext_tsk\n"  // tk_ext_tsk() is called
      "-- tk_ext_tsk\n"  //  because the task action is not infinite loop.
      "- tk_sta_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(TaskTest, SuspendSelfTask) {
  task->Suspend(t);

  EXPECT_EQ(TMO_FEVR, utkernelTskSpy->Timeout());
  EXPECT_STREQ(
      "+ tk_get_tid\n"
      "- tk_get_tid (0)\n"
      "+ tk_slp_tsk\n"
      "- tk_slp_tsk (0)\n",  // Return because tk_slp_tsk() is test double.
      systemCallLogger->Get());
}

TEST_F(TaskTest, SuspendOtherTask) {
  utkernelTskSpy->SetReturnCode(0, 1);

  task->Suspend(t);

  EXPECT_STREQ(
      "+ tk_get_tid\n"
      "- tk_get_tid (1)\n"
      "+ tk_sus_tsk (0)\n"
      "- tk_sus_tsk (0)\n"
      "+ tk_rel_wai (0)\n"
      "- tk_rel_wai (0)\n",
      systemCallLogger->Get());
}

TEST_F(TaskTest, SuspendMultipleTimes) {
  task->Suspend(t);
  task->Suspend(t);
  task->Suspend(t);

  EXPECT_STREQ(
      "+ tk_get_tid\n"
      "- tk_get_tid (0)\n"
      "+ tk_slp_tsk\n"
      "- tk_slp_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(TaskTest, ResumeTaskSuspendedByMyself) {
  task->Suspend(t);
  systemCallLogger->Reset();

  task->Resume(t);

  EXPECT_STREQ(
      "+ tk_wup_tsk (0)\n"
      "- tk_wup_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(TaskTest, ResumeTaskSuspendedByOther) {
  utkernelTskSpy->SetReturnCode(0, 1);
  task->Suspend(t);
  systemCallLogger->Reset();

  task->Resume(t);

  EXPECT_STREQ(
      "+ tk_rsm_tsk (0)\n"
      "- tk_rsm_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(TaskTest, ResumeTaskNotSuspended) {
  task->Resume(t);

  EXPECT_STREQ("", systemCallLogger->Get());
}

TEST_F(TaskTest, ResumeMultipleTimes) {
  task->Suspend(t);
  systemCallLogger->Reset();

  task->Resume(t);
  task->Resume(t);
  task->Resume(t);

  EXPECT_STREQ(
      "+ tk_wup_tsk (0)\n"
      "- tk_wup_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(TaskTest, Delay) {
  task->Delay(100);

  EXPECT_EQ(100, utkernelTskSpy->DelayTime());
  EXPECT_STREQ(
      "+ tk_dly_tsk\n"
      "- tk_dly_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(TaskTest, DelayWithTimeZeroOrLess) {
  task->Delay(0);
  task->Delay(-100);

  EXPECT_STREQ("", systemCallLogger->Get());
}
