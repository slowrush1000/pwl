/**
 * @file pwl_main.cpp
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
#include <iostream>
#include <string>

pwl::Version k_version(std::string("pwl.exe"), 20230402, 0, 0);

int
main()
{
    std::cout << k_version.str() << "\n";

    return 0;
}