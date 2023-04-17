/**
 * @file pwl_git.cpp
 * @author Cheon Younghoe (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-04-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "pwl_git.hpp"

pwl::Git::~Git() {}

std::string pwl::Git::getGitBranch() { return kGitBranch; }

std::string pwl::Git::getGitCommitHash() { return kGitCommitHash; }
