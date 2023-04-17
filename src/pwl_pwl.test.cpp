/**
 * @file pwl_pwl.test.cpp
 * @author Cheon Younghoe (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-04-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "pwl_pwl.hpp"
#include "pwl_util.hpp"
#include "gtest/gtest.h"

TEST(PWL_TEST, horizontalLine) {
  pwl::PWL myPWL;
  myPWL.addXY(pwl::XY(0.0, 1.0));
  myPWL.addXY(pwl::XY(1.0e-6, 1.0));
  myPWL.addXY(pwl::XY(4.0e-6, 1.0));
  myPWL.addXY(pwl::XY(1.0e-5, 1.0));

  EXPECT_TRUE(pwl::isEqual(1.0, myPWL.getY(2.0e-6)));
  EXPECT_TRUE(pwl::isEqual(1.0, myPWL.getY(2.0e-6)));
  EXPECT_TRUE(pwl::isEqual(1.0, myPWL.getAvg(1.0e-6, 7.0e-6)));
  EXPECT_TRUE(pwl::isEqual(1.0, myPWL.getRMS(1.0e-6, 7.0e-6)));
  EXPECT_TRUE(pwl::isEqual(1.0, myPWL.getMax(1.0e-6, 7.0e-6)));
  EXPECT_TRUE(pwl::isEqual(1.0, myPWL.getMin(1.0e-6, 7.0e-6)));
  EXPECT_TRUE(pwl::isEqual(0.0, myPWL.getMaxPeakToPeak(1.0e-6, 7.0e-6)));
  EXPECT_EQ(1, std::isnan(myPWL.getY(-1.0e-6)));
  EXPECT_EQ(1, std::isnan(myPWL.getY(2.0e-5)));
}

TEST(PWL_TEST, trangleLine) {
  pwl::PWL myPWL;
  myPWL.addXY(pwl::XY(0.0, 0.0));
  myPWL.addXY(pwl::XY(1.0e-5, 100.0));

  EXPECT_DOUBLE_EQ(0.0, myPWL.getY(0.0));
  EXPECT_DOUBLE_EQ(0.0, myPWL.getY(0.0));
  EXPECT_DOUBLE_EQ(50.0, myPWL.getY(0.5e-5));
  EXPECT_DOUBLE_EQ(100.0, myPWL.getY(1.0e-5));
  EXPECT_DOUBLE_EQ(50.0, myPWL.getAvg(0.0, 1.0e-5));
  EXPECT_DOUBLE_EQ(0.0, myPWL.getMin(0.0, 1.0e-5));
  EXPECT_DOUBLE_EQ(100.0, myPWL.getMax(0.0, 1.0e-5));
  EXPECT_DOUBLE_EQ(100.0, myPWL.getMaxPeakToPeak(0.0, 1.0e-5));
}

TEST(PWL_TEST, i1vin1) {
  pwl::PWL myPWL;
  myPWL.readFile(std::string("../../data/i1vin.txt"), 1, 2);
  myPWL.writeFile(std::string("i1vin.write.txt"));

  /*
  EXPECT_TRUE(pwl::isEqual(-2.678997e-11, myPWL.getAvg(5.05e-8, 5.15e-8)));
  EXPECT_TRUE(pwl::isEqual(-5.982921e-02, myPWL.getMin(5.05e-8, 5.15e-8)));
  EXPECT_TRUE(pwl::isEqual(5.965893e-02,  myPWL.getMax(5.05e-8, 5.15e-8)));
  EXPECT_TRUE(pwl::isEqual(1.194881e-01,
  myPWL.getMaxPeakToPeak(5.05e-8, 5.15e-8)));
  EXPECT_TRUE(pwl::isEqual(2.31769e-02,   myPWL.getRMS(5.05e-8, 5.15e-8)));
  */
  EXPECT_DOUBLE_EQ(-2.678997e-11, myPWL.getAvg(5.05e-8, 5.15e-8));
  EXPECT_DOUBLE_EQ(-5.982921e-02, myPWL.getMin(5.05e-8, 5.15e-8));
  EXPECT_DOUBLE_EQ(5.965893e-02, myPWL.getMax(5.05e-8, 5.15e-8));
  EXPECT_DOUBLE_EQ(1.194881e-01, myPWL.getMaxPeakToPeak(5.05e-8, 5.15e-8));
  EXPECT_DOUBLE_EQ(2.31769e-02, myPWL.getRMS(5.05e-8, 5.15e-8));
}

TEST(PWL_TEST, vout1) {
  pwl::PWL myPWL;
  myPWL.readFile(std::string("../../data/vout.txt"), 1, 2);
  myPWL.writeFile(std::string("vout.write.txt"));

  /*
  EXPECT_TRUE(pwl::isEqual(-2.678997e-11, myPWL.getAvg(5.05e-8, 5.15e-8)));
  EXPECT_TRUE(pwl::isEqual(-5.982921e-02, myPWL.getMin(5.05e-8, 5.15e-8)));
  EXPECT_TRUE(pwl::isEqual(5.965893e-02,  myPWL.getMax(5.05e-8, 5.15e-8)));
  EXPECT_TRUE(pwl::isEqual(1.194881e-01,
  myPWL.getMaxPeakToPeak(5.05e-8, 5.15e-8)));
  EXPECT_TRUE(pwl::isEqual(2.31769e-02,   myPWL.getRMS(5.05e-8, 5.15e-8)));
  */
  EXPECT_DOUBLE_EQ(1.001001e-01, myPWL.getAvg(5.05e-8, 5.15e-8));
  EXPECT_DOUBLE_EQ(2.431209e-248, myPWL.getMin(5.05e-8, 5.15e-8));
  EXPECT_DOUBLE_EQ(8.801812e-01, myPWL.getMax(5.05e-8, 5.15e-8));
  EXPECT_DOUBLE_EQ(8.801812e-01, myPWL.getMaxPeakToPeak(5.05e-8, 5.15e-8));
  EXPECT_DOUBLE_EQ(2.49450e-01, myPWL.getRMS(5.05e-8, 5.15e-8));
}