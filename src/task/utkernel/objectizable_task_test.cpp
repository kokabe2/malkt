// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "objectizable_task.h"
#include "util/system_call_logger.h"
#include "utkernel_tsk_spy.h"
}

namespace {
bool was_ran;
void *given_object;

void DummyAction(void *object) {
  was_ran = true;
  given_object = object;
}
}

class ObjectizableTaskTest : public ::testing::Test {
 protected:
  Task task;
  void *dummy_object;

  virtual void SetUp() {
    was_ran = false;
    given_object = NULL;
    utkernelTskSpy->Reset();
    systemCallLogger->Reset();
  }

  virtual void TearDown() {
    if (task != NULL) task->Delete(&task);
  }

  void NewObjectizableTask() {
    task = objectizableTask->New(DummyAction, 4, kMaxTaskStackSize, dummy_object);
    systemCallLogger->Reset();
  }
};

TEST_F(ObjectizableTaskTest, New) {
  task = objectizableTask->New(DummyAction, 4, kMaxTaskStackSize, dummy_object);

  ASSERT_TRUE(task != NULL);
  EXPECT_EQ(TA_HLNG | TA_RNG0, utkernelTskSpy->Attribute());
  EXPECT_EQ(9, utkernelTskSpy->Priority());
  EXPECT_EQ(kMaxTaskStackSize, utkernelTskSpy->StackSize());
  EXPECT_FALSE(was_ran);
  EXPECT_EQ(NULL, given_object);
  EXPECT_STREQ(
      "+ tk_cre_tsk\n"
      "- tk_cre_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(ObjectizableTaskTest, NewWithBoundaryPriority) {
  task = objectizableTask->New(DummyAction, kHighestTaskPriority, kMaxTaskStackSize, NULL);
  EXPECT_EQ(5, utkernelTskSpy->Priority());
  task->Delete(&task);

  task = objectizableTask->New(DummyAction, kLowestTaskPriority, kMaxTaskStackSize, NULL);
  EXPECT_EQ(12, utkernelTskSpy->Priority());
}

TEST_F(ObjectizableTaskTest, NewWithOutOfRangePriority) {
  task = objectizableTask->New(DummyAction, kHighestTaskPriority + 1, kMaxTaskStackSize, NULL);
  EXPECT_EQ(5, utkernelTskSpy->Priority());
  task->Delete(&task);

  task = objectizableTask->New(DummyAction, kLowestTaskPriority - 1, kMaxTaskStackSize, NULL);
  EXPECT_EQ(12, utkernelTskSpy->Priority());
}

TEST_F(ObjectizableTaskTest, NewWithOutOfRangeStackSize) {
  task = objectizableTask->New(DummyAction, 4, kMaxTaskStackSize + 1, NULL);

  EXPECT_EQ(kMaxTaskStackSize, utkernelTskSpy->StackSize());
}

TEST_F(ObjectizableTaskTest, DeleteSelfTask) {
  NewObjectizableTask();

  task->Delete(&task);

  EXPECT_EQ(NULL, task);
  EXPECT_STREQ(
      "+ tk_get_tid\n"
      "- tk_get_tid (0)\n"
      "+ tk_exd_tsk\n"
      "- tk_exd_tsk\n",
      systemCallLogger->Get());
}

TEST_F(ObjectizableTaskTest, DeleteOtherTask) {
  NewObjectizableTask();
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

TEST_F(ObjectizableTaskTest, Run) {
  NewObjectizableTask();

  task->Run(task);

  EXPECT_TRUE(was_ran);
  EXPECT_EQ(dummy_object, given_object);
  EXPECT_STREQ(
      "+ tk_sta_tsk (0)\n"
      "++ tk_ext_tsk\n"  // tk_ext_tsk() is called
      "-- tk_ext_tsk\n"  //  because the task action is not infinite loop.
      "- tk_sta_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(ObjectizableTaskTest, SuspendSelfTask) {
  NewObjectizableTask();

  task->Suspend(task);

  EXPECT_EQ(TMO_FEVR, utkernelTskSpy->Timeout());
  EXPECT_STREQ(
      "+ tk_get_tid\n"
      "- tk_get_tid (0)\n"
      "+ tk_slp_tsk\n"
      "- tk_slp_tsk (0)\n",  // Return because tk_slp_tsk() is test double.
      systemCallLogger->Get());
}

TEST_F(ObjectizableTaskTest, SuspendOtherTask) {
  NewObjectizableTask();
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

TEST_F(ObjectizableTaskTest, SuspendMultipleTimes) {
  NewObjectizableTask();

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

TEST_F(ObjectizableTaskTest, ResumeTaskSuspendedByMyself) {
  NewObjectizableTask();
  task->Suspend(task);
  systemCallLogger->Reset();

  task->Resume(task);

  EXPECT_STREQ(
      "+ tk_wup_tsk (0)\n"
      "- tk_wup_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(ObjectizableTaskTest, ResumeTaskSuspendedByOther) {
  NewObjectizableTask();
  utkernelTskSpy->SetReturnCode(0, 1);
  task->Suspend(task);
  systemCallLogger->Reset();

  task->Resume(task);

  EXPECT_STREQ(
      "+ tk_rsm_tsk (0)\n"
      "- tk_rsm_tsk (0)\n",
      systemCallLogger->Get());
}

TEST_F(ObjectizableTaskTest, ResumeTaskNotSuspended) {
  NewObjectizableTask();

  task->Resume(task);

  EXPECT_STREQ("", systemCallLogger->Get());
}

TEST_F(ObjectizableTaskTest, ResumeMultipleTimes) {
  NewObjectizableTask();
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
