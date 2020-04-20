// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "simple_task.h"
#include "util/system_call_logger.h"
#include "utkernel_tsk_spy.h"
}

namespace {
bool was_ran;

void DummyAction(void) { was_ran = true; }
}

class SimpleTaskTest : public ::testing::Test {
 protected:
  Task task;

  virtual void SetUp() {
    was_ran = false;
    utkernelTskSpy->Reset();
    systemCallLogger->Reset();
  }

  virtual void TearDown() {
    if (task != NULL) task->Delete(&task);
  }

  void NewSimpleTask() {
    task = simpleTask->New(DummyAction, 4, kMaxTaskStackSize);
    systemCallLogger->Reset();
  }
};

TEST_F(SimpleTaskTest, New) {
  task = simpleTask->New(DummyAction, 4, kMaxTaskStackSize);

  ASSERT_TRUE(task != NULL);
  EXPECT_EQ(TA_HLNG | TA_RNG0, utkernelTskSpy->Attribute());
  EXPECT_EQ(9, utkernelTskSpy->Priority());
  EXPECT_EQ(kMaxTaskStackSize, utkernelTskSpy->StackSize());
  EXPECT_FALSE(was_ran);
  EXPECT_STREQ(
      "+ tk_cre_tsk\n"
      "- tk_cre_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(SimpleTaskTest, NewWithBoundaryPriority) {
  task = simpleTask->New(DummyAction, kHighestTaskPriority, kMaxTaskStackSize);
  EXPECT_EQ(5, utkernelTskSpy->Priority());
  task->Delete(&task);

  task = simpleTask->New(DummyAction, kLowestTaskPriority, kMaxTaskStackSize);
  EXPECT_EQ(12, utkernelTskSpy->Priority());
}

TEST_F(SimpleTaskTest, NewWithOutOfRangePriority) {
  task = simpleTask->New(DummyAction, kHighestTaskPriority + 1, kMaxTaskStackSize);
  EXPECT_EQ(5, utkernelTskSpy->Priority());
  task->Delete(&task);

  task = simpleTask->New(DummyAction, kLowestTaskPriority - 1, kMaxTaskStackSize);
  EXPECT_EQ(12, utkernelTskSpy->Priority());
}

TEST_F(SimpleTaskTest, NewWithOutOfRangeStackSize) {
  task = simpleTask->New(DummyAction, 4, kMaxTaskStackSize + 1);

  EXPECT_EQ(kMaxTaskStackSize, utkernelTskSpy->StackSize());
}

TEST_F(SimpleTaskTest, DeleteSelfTask) {
  NewSimpleTask();

  task->Delete(&task);

  EXPECT_EQ(NULL, task);
  EXPECT_STREQ(
      "+ tk_get_tid\n"
      "- tk_get_tid (0)\n"
      "+ tk_exd_tsk\n"
      "- tk_exd_tsk\n",
      systemCallLogger->Get());
}

TEST_F(SimpleTaskTest, DeleteOtherTask) {
  NewSimpleTask();
  utkernelTskSpy->SetReturnCode(0, 1);

  task->Delete(&task);

  EXPECT_EQ(NULL, task);
  EXPECT_STREQ(
      "+ tk_get_tid\n"
      "- tk_get_tid (1)\n"
      "+ tk_ter_tsk (0)\n"
      "- tk_ter_tsk (0)\n"
      "+ tk_del_tsk (0)\n"
      "- tk_del_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(SimpleTaskTest, Run) {
  NewSimpleTask();

  task->Run(task);

  EXPECT_TRUE(was_ran);
  EXPECT_STREQ(
      "+ tk_sta_tsk (0)\n"
      "++ tk_ext_tsk\n"  // tk_ext_tsk() is called
      "-- tk_ext_tsk\n"  //  because the task action is not infinite loop.
      "- tk_sta_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(SimpleTaskTest, SuspendSelfTask) {
  NewSimpleTask();

  task->Suspend(task);

  EXPECT_EQ(TMO_FEVR, utkernelTskSpy->Timeout());
  EXPECT_STREQ(
      "+ tk_get_tid\n"
      "- tk_get_tid (0)\n"
      "+ tk_slp_tsk\n"
      "- tk_slp_tsk (0)\n",  // Return because tk_slp_tsk() is test double.
      systemCallLogger->Get());
}

TEST_F(SimpleTaskTest, SuspendOtherTask) {
  NewSimpleTask();
  utkernelTskSpy->SetReturnCode(0, 1);

  task->Suspend(task);

  EXPECT_STREQ(
      "+ tk_get_tid\n"
      "- tk_get_tid (1)\n"
      "+ tk_sus_tsk (0)\n"
      "- tk_sus_tsk (0)\n"
      "+ tk_rel_wai (0)\n"
      "- tk_rel_wai (0)\n",
      systemCallLogger->Get());
}

TEST_F(SimpleTaskTest, SuspendMultipleTimes) {
  NewSimpleTask();

  task->Suspend(task);
  task->Suspend(task);
  task->Suspend(task);

  EXPECT_STREQ(
      "+ tk_get_tid\n"
      "- tk_get_tid (0)\n"
      "+ tk_slp_tsk\n"
      "- tk_slp_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(SimpleTaskTest, ResumeTaskSuspendedByMyself) {
  NewSimpleTask();
  task->Suspend(task);
  systemCallLogger->Reset();

  task->Resume(task);

  EXPECT_STREQ(
      "+ tk_wup_tsk (0)\n"
      "- tk_wup_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(SimpleTaskTest, ResumeTaskSuspendedByOther) {
  NewSimpleTask();
  utkernelTskSpy->SetReturnCode(0, 1);
  task->Suspend(task);
  systemCallLogger->Reset();

  task->Resume(task);

  EXPECT_STREQ(
      "+ tk_rsm_tsk (0)\n"
      "- tk_rsm_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(SimpleTaskTest, ResumeTaskNotSuspended) {
  NewSimpleTask();

  task->Resume(task);

  EXPECT_STREQ("", systemCallLogger->Get());
}

TEST_F(SimpleTaskTest, ResumeMultipleTimes) {
  NewSimpleTask();
  task->Suspend(task);
  systemCallLogger->Reset();

  task->Resume(task);
  task->Resume(task);
  task->Resume(task);

  EXPECT_STREQ(
      "+ tk_wup_tsk (0)\n"
      "- tk_wup_tsk (0)\n",
      systemCallLogger->Get());
}
