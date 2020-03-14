// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "uptime.h"
#include "utkernel/utkernel.h"
}

TEST(UptimeTest, Get) {
  SYSTIM pk_tim = {0x01234567, 0x89ABCDEF};
  tk_set_tim(&pk_tim);
  EXPECT_EQ(0x0123456789ABCDEF, uptime->Get());

  pk_tim.lo += 1;
  tk_set_tim(&pk_tim);
  EXPECT_EQ(0x0123456789ABCDF0, uptime->Get());

  pk_tim.hi += 0x12345678;
  pk_tim.lo += 0x90000000;
  tk_set_tim(&pk_tim);
  EXPECT_EQ(0x13579BE019ABCDF0, uptime->Get());

  pk_tim.hi += 0x90000000;
  pk_tim.lo += 0x00000001;
  tk_set_tim(&pk_tim);
  EXPECT_EQ(0xA3579BE019ABCDF1, uptime->Get());
}

TEST(UptimeTest, Set) {
  uptime->Set(0xFFFFFFFFFFFFFFFF);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, uptime->Get());

  uptime->Set(0xA3579BE019ABCDF1);
  EXPECT_EQ(0xA3579BE019ABCDF1, uptime->Get());

  uptime->Set(0);
  EXPECT_EQ(0, uptime->Get());
}
