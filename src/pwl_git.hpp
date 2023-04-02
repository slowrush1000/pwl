/**
 * @file pwl_git.hpp.in
 * @author Cheon Younghoe (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PWL_GIT_H
#define PWL_GIT_H

#include <string>

namespace pwl
{
    const std::string   kGitBranch          = "HEAD";
    const std::string   kGitCommitHash      = "";

	class Git
	{
		public:
			Git() = default;
			virtual ~Git();

    		static std::string  getGitBranch();
            static std::string  getGitCommitHash();
	};
}

#endif // PWL_GIT_H
