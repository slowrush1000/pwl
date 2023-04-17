/**
 * @file pwl_main.test.cpp
 * @author Cheon Younghoe (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-04-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "pwl_main.hpp"
#include "pwl_version.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <string>

pwl::Version kVersion(std::string("pwl.test.exe"), 20230402, 0, 0);

int main(int argc, char *argv[]) {
  std::cout << kVersion.getStr() << "\n";

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
