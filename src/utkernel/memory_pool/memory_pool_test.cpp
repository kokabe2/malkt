
// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "../util/system_call_logger.h"
#include "memory_pool.h"
#include "utkernel_mpf_spy.h"
}

namespace {
char memory_area[512 + 1];
}  // namespace

class MemoryPoolTest : public ::testing::Test {
 protected:
  MemoryPool mp;
  virtual void SetUp() {
    utkernelMpfSpy->Reset();
    mp = memoryPool->New(memory_area, sizeof(memory_area), 32);
    systemCallLogger->Reset();
  }
  virtual void TearDown() { memoryPool->Delete(&mp); }
};

TEST_F(MemoryPoolTest, New) {
  MemoryPool instance = memoryPool->New(memory_area, sizeof(memory_area), 32);

  ASSERT_TRUE(instance != NULL);
  EXPECT_EQ((TA_TFIFO | TA_RNG0 | TA_USERBUF), utkernelMpfSpy->Attribute());
  EXPECT_EQ(16, utkernelMpfSpy->BlockCount());
  EXPECT_EQ(32, utkernelMpfSpy->BlockSize());
  EXPECT_EQ(memory_area, utkernelMpfSpy->UserBuffer());
  EXPECT_STREQ(
      "+ tk_cre_mpf\n"
      "- tk_cre_mpf (0)\n",
      systemCallLogger->Get());

  memoryPool->Delete(&instance);
}

TEST_F(MemoryPoolTest, NewWhenMemoryPoolCreationFailed) {
  utkernelMpfSpy->SetReturnCode(0, -34);

  EXPECT_EQ(NULL, memoryPool->New(memory_area, sizeof(memory_area), 32));
  EXPECT_STREQ(
      "+ tk_cre_mpf\n"
      "- tk_cre_mpf (-34)\n",
      systemCallLogger->Get());
}

TEST_F(MemoryPoolTest, NewWithInvalidArgument) {
  EXPECT_EQ(NULL, memoryPool->New(NULL, sizeof(memory_area), 32));
  EXPECT_EQ(NULL, memoryPool->New(memory_area, 0, 32));
  EXPECT_EQ(NULL, memoryPool->New(memory_area, -128, 32));
  EXPECT_EQ(NULL, memoryPool->New(memory_area, sizeof(memory_area), 0));
  EXPECT_EQ(NULL, memoryPool->New(memory_area, sizeof(memory_area), -8));
  EXPECT_EQ(NULL, memoryPool->New(memory_area, sizeof(memory_area), sizeof(memory_area) + 1));
  EXPECT_STREQ("", systemCallLogger->Get());
}

TEST_F(MemoryPoolTest, Delete) {
  memoryPool->Delete(&mp);

  EXPECT_EQ(NULL, mp);
  EXPECT_STREQ(
      "+ tk_del_mpf (0)\n"
      "- tk_del_mpf (0)\n",
      systemCallLogger->Get());
}

TEST_F(MemoryPoolTest, DeleteMultipleTimes) {
  memoryPool->Delete(&mp);
  systemCallLogger->Reset();

  memoryPool->Delete(&mp);

  EXPECT_STREQ("", systemCallLogger->Get());
}

TEST_F(MemoryPoolTest, Get) {
  EXPECT_TRUE(memoryPool->Get(mp) != NULL);
  EXPECT_EQ(TMO_POL, utkernelMpfSpy->Timout());
  EXPECT_STREQ(
      "+ tk_get_mpf (0)\n"
      "- tk_get_mpf (0)\n",
      systemCallLogger->Get());
}

TEST_F(MemoryPoolTest, GetWhenMemoryBlockAcquisitionFailed) {
  utkernelMpfSpy->SetReturnCode(0, -50);

  EXPECT_EQ(NULL, memoryPool->Get(mp));
  EXPECT_STREQ(
      "+ tk_get_mpf (0)\n"
      "- tk_get_mpf (-50)\n",
      systemCallLogger->Get());
}

TEST_F(MemoryPoolTest, Release) {
  void *block = memoryPool->Get(mp);
  systemCallLogger->Reset();

  memoryPool->Release(mp, block);

  EXPECT_STREQ(
      "+ tk_rel_mpf (0)\n"
      "- tk_rel_mpf (0)\n",
      systemCallLogger->Get());
}

TEST_F(MemoryPoolTest, ReleaseWithInvalidArgument) {
  memoryPool->Release(mp, NULL);

  EXPECT_STREQ("", systemCallLogger->Get());
}

TEST_F(MemoryPoolTest, CallMethodWithNullInstance) {
  void *block = memoryPool->Get(mp);
  systemCallLogger->Reset();

  memoryPool->Delete(NULL);
  EXPECT_EQ(NULL, memoryPool->Get(NULL));
  memoryPool->Release(NULL, block);

  EXPECT_STREQ("", systemCallLogger->Get());
}
