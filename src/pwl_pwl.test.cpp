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
#include "gtest/gtest.h"

TEST(PWL_TEST, horizontalLine)
{
    pwl::PWL            myPWL;
    myPWL.addXY(pwl::XY(0.0, 1.0));
    myPWL.addXY(pwl::XY(1.0e-6, 1.0));
    myPWL.addXY(pwl::XY(4.0e-6, 1.0));
    myPWL.addXY(pwl::XY(1.0e-5, 1.0));

    EXPECT_EQ(1.0, myPWL.getY(2.0e-6));
    EXPECT_EQ(1.0, myPWL.getAvg(1.0e-6, 7.0e-6));
    EXPECT_EQ(1.0, myPWL.getRMS(1.0e-6, 7.0e-6));
    EXPECT_EQ(1.0, myPWL.getMax(1.0e-6, 7.0e-6));
    EXPECT_EQ(1.0, myPWL.getMin(1.0e-6, 7.0e-6));
    EXPECT_EQ(0.0, myPWL.getMaxPeakToPeak(1.0e-6, 7.0e-6));
    EXPECT_EQ(1, std::isnan(myPWL.getY(-1.0e-6)));
    EXPECT_EQ(1, std::isnan(myPWL.getY(2.0e-5)));
}