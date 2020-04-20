
// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "basic_memory_pool.h"
#include "util/system_call_logger.h"
#include "utkernel_mpf_spy.h"
}

namespace {
char memory_area[512 + 1];
}  // namespace

class BasicMemoryPoolTest : public ::testing::Test {
 protected:
  MemoryPool mp;

  virtual void SetUp() {
    utkernelMpfSpy->Reset();
    systemCallLogger->Reset();
  }

  virtual void TearDown() {
    if (mp != NULL) mp->Delete(&mp);
  }

  void NewBasicMemoryPool() {
    mp = basicMemoryPool->New(memory_area, sizeof(memory_area), 32);
    systemCallLogger->Reset();
  }
};

TEST_F(BasicMemoryPoolTest, New) {
  mp = basicMemoryPool->New(memory_area, sizeof(memory_area), 32);

  ASSERT_TRUE(mp != NULL);
  EXPECT_EQ((TA_TFIFO | TA_RNG0 | TA_USERBUF), utkernelMpfSpy->Attribute());
  EXPECT_EQ(16, utkernelMpfSpy->BlockCount());
  EXPECT_EQ(32, utkernelMpfSpy->BlockSize());
  EXPECT_EQ(memory_area, utkernelMpfSpy->UserBuffer());
  EXPECT_STREQ(
      "+ tk_cre_mpf\n"
      "- tk_cre_mpf (0)\n",
      systemCallLogger->Get());
}

TEST_F(BasicMemoryPoolTest, Delete) {
  NewBasicMemoryPool();

  mp->Delete(&mp);

  EXPECT_EQ(NULL, mp);
  EXPECT_STREQ(
      "+ tk_del_mpf (0)\n"
      "- tk_del_mpf (0)\n",
      systemCallLogger->Get());
}

TEST_F(BasicMemoryPoolTest, Get) {
  NewBasicMemoryPool();

  EXPECT_TRUE(mp->Get(mp) != NULL);
  EXPECT_EQ(TMO_POL, utkernelMpfSpy->Timout());
  EXPECT_STREQ(
      "+ tk_get_mpf (0)\n"
      "- tk_get_mpf (0)\n",
      systemCallLogger->Get());
}

TEST_F(BasicMemoryPoolTest, GetWhenMemoryBlockAcquisitionFailed) {
  NewBasicMemoryPool();
  utkernelMpfSpy->SetReturnCode(0, -50);

  EXPECT_EQ(NULL, mp->Get(mp));
  EXPECT_STREQ(
      "+ tk_get_mpf (0)\n"
      "- tk_get_mpf (-50)\n",
      systemCallLogger->Get());
}

TEST_F(BasicMemoryPoolTest, Release) {
  NewBasicMemoryPool();
  void *block = mp->Get(mp);
  systemCallLogger->Reset();

  mp->Release(mp, block);

  EXPECT_STREQ(
      "+ tk_rel_mpf (0)\n"
      "- tk_rel_mpf (0)\n",
      systemCallLogger->Get());
}
