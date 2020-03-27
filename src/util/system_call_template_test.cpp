// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "system_call_logger.h"
#include "system_call_template.h"
}

namespace {
bool was_run;
const void* given_info;

void ExecuteSpy(const void* info) {
  was_run = true;
  given_info = info;
}
}  // namespace

class SystemCallTemplateTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    systemCallLogger->Reset();
    was_run = false;
    given_info = NULL;
  }

  virtual void TearDown() {}
};

TEST_F(SystemCallTemplateTest, ExecuteWithNoOption) {
  systemCallTemplate->Execute("test", NULL, NULL);

  EXPECT_STREQ(
      "+ test\n"
      "- test\n",
      systemCallLogger->Get());
}

TEST_F(SystemCallTemplateTest, ExecuteWithId) {
  systemCallTemplate->SetId(6);
  systemCallTemplate->Execute("test", NULL, NULL);

  EXPECT_STREQ(
      "+ test (6)\n"
      "- test\n",
      systemCallLogger->Get());
}

TEST_F(SystemCallTemplateTest, ExecuteWithReturnCode) {
  systemCallTemplate->SetReturnCode(-34);

  EXPECT_EQ(-34, systemCallTemplate->Execute("test", NULL, NULL));
  EXPECT_STREQ(
      "+ test\n"
      "- test (-34)\n",
      systemCallLogger->Get());
}

TEST_F(SystemCallTemplateTest, ExecuteWithExecuteDelegate) {
  int info = 128;

  systemCallTemplate->Execute("test", ExecuteSpy, &info);

  EXPECT_TRUE(was_run);
  EXPECT_EQ(&info, given_info);
  EXPECT_STREQ(
      "+ test\n"
      "- test\n",
      systemCallLogger->Get());
}

TEST_F(SystemCallTemplateTest, ExecuteWithExecuteDelegateWhenErrorIsSet) {
  systemCallTemplate->SetReturnCode(-34);
  systemCallTemplate->Execute("test", ExecuteSpy, NULL);

  EXPECT_FALSE(was_run);
  EXPECT_STREQ(
      "+ test\n"
      "- test (-34)\n",
      systemCallLogger->Get());
}
