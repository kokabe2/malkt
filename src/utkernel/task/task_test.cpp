// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "../util/system_call_logger.h"
#include "function_entry_spy.h"
#include "task.h"
#include "utkernel_task_spy.h"
}

class TaskTest : public ::testing::Test {
 protected:
  Task t;
  virtual void SetUp() {
    functionEntrySpy->Reset();
    utkernelTaskSpy->Reset();
    systemCallLogger->Reset();
    t = task->New(functionEntrySpy->Get(), 4, kMaxTaskStackSize);
  }
  virtual void TearDown() { task->Delete(&t); }
};

TEST_F(TaskTest, ConditionAfterNew) {
  ASSERT_TRUE(t != NULL);
  EXPECT_EQ(TA_HLNG | TA_RNG0, utkernelTaskSpy->Attribute());
  EXPECT_EQ(9, utkernelTaskSpy->Priority());
  EXPECT_EQ(kMaxTaskStackSize, utkernelTaskSpy->StackSize());
  EXPECT_FALSE(functionEntrySpy->WasRun());
  EXPECT_STREQ(
      "+ tk_cre_tsk\n"
      "- tk_cre_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(TaskTest, NewWithBoundaryPriority) {
  Task highest_priority = task->New(functionEntrySpy->Get(),
                                    kHighestTaskPriority, kMaxTaskStackSize);
  EXPECT_EQ(5, utkernelTaskSpy->Priority());
  task->Delete(&highest_priority);

  Task lowest_priority = task->New(functionEntrySpy->Get(), kLowestTaskPriority,
                                   kMaxTaskStackSize);
  EXPECT_EQ(12, utkernelTaskSpy->Priority());
  task->Delete(&lowest_priority);
}

TEST_F(TaskTest, NewWhenTaskCreationFailed) {
  utkernelTaskSpy->SetReturnCode(0, -34);
  systemCallLogger->Reset();

  EXPECT_EQ(NULL, task->New(functionEntrySpy->Get(), 4, kMaxTaskStackSize));
  EXPECT_STREQ(
      "+ tk_cre_tsk\n"
      "- tk_cre_tsk (-34)\n",
      systemCallLogger->Get());
}

TEST_F(TaskTest, NewWithInvalidArguments) {
  EXPECT_EQ(NULL, task->New(NULL, 4, kMaxTaskStackSize));
  EXPECT_EQ(NULL, task->New(functionEntrySpy->Get(), kLowestTaskPriority - 1,
                            kMaxTaskStackSize));
  EXPECT_EQ(NULL, task->New(functionEntrySpy->Get(), kHighestTaskPriority + 1,
                            kMaxTaskStackSize));
  EXPECT_EQ(NULL, task->New(functionEntrySpy->Get(), 4, 0));
  EXPECT_EQ(NULL, task->New(functionEntrySpy->Get(), 4, kMaxTaskStackSize + 1));
}

TEST_F(TaskTest, DeleteSelfTask) {
  systemCallLogger->Reset();

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
  utkernelTaskSpy->SetReturnCode(0, 1);
  systemCallLogger->Reset();

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

TEST_F(TaskTest, DeleteMultipleTimes) {
  task->Delete(&t);
  task->Delete(&t);

  EXPECT_EQ(NULL, t);
}

TEST_F(TaskTest, Run) {
  systemCallLogger->Reset();

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
  systemCallLogger->Reset();

  task->Suspend(t);

  EXPECT_EQ(TMO_FEVR, utkernelTaskSpy->Timeout());
  EXPECT_STREQ(
      "+ tk_get_tid\n"
      "- tk_get_tid (0)\n"
      "+ tk_slp_tsk\n"
      "- tk_slp_tsk (0)\n",  // Return because tk_slp_tsk() is test double.
      systemCallLogger->Get());
}

TEST_F(TaskTest, SuspendOtherTask) {
  utkernelTaskSpy->SetReturnCode(0, 1);
  systemCallLogger->Reset();

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
  systemCallLogger->Reset();

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
  utkernelTaskSpy->SetReturnCode(0, 1);
  task->Suspend(t);
  systemCallLogger->Reset();

  task->Resume(t);

  EXPECT_STREQ(
      "+ tk_rsm_tsk (0)\n"
      "- tk_rsm_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(TaskTest, ResumeTaskNotSuspended) {
  systemCallLogger->Reset();

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
  systemCallLogger->Reset();

  task->Delay(100);

  EXPECT_EQ(100, utkernelTaskSpy->DelayTime());
  EXPECT_STREQ(
      "+ tk_dly_tsk\n"
      "- tk_dly_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(TaskTest, DelayWithTimeZeroOrLess) {
  systemCallLogger->Reset();

  task->Delay(0);
  task->Delay(-100);

  EXPECT_STREQ("", systemCallLogger->Get());
}

TEST_F(TaskTest, CallMethodWithNullInstance) {
  task->Delete(NULL);
  task->Run(NULL);
  task->Suspend(NULL);
  task->Resume(NULL);

  SUCCEED();
}
