/**
 * @file pwl_version.cpp
 * @author Cheon Younghoe (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "pwl_version.hpp"
#include <fmt/printf.h>

pwl::Version::Version(const std::string program, const int majorVersion, const int minorVersion, const int patchVersion, const std::string buildDate, const std::string buildTime) : mProgram(program), mMajorVersion(majorVersion), mMinorversion(minorVersion), mPatchVersion(patchVersion), mBuildDate(buildDate), mBuildTime(buildTime)
{
}

pwl::Version::Version(const pwl::Version& other)
{
    mProgram        = other.mProgram;
    mMajorVersion   = other.mMajorVersion;
    mMinorversion   = other.mMinorversion;
    mPatchVersion   = other.mPatchVersion;
    mBuildDate      = other.mBuildDate;
    mBuildTime      = other.mBuildTime;
}

pwl::Version::~Version()
{
}

const std::string&  pwl::Version::getProgram() const
{
    return mProgram;
}

int                 
pwl::Version::getMajorVersion() const
{
    return mMajorVersion;
}

int                 
pwl::Version::getMinorVersion() const
{
    return mMinorversion;
}

int                 
pwl::Version::getPatchVersion() const
{
    return mPatchVersion;
}

const std::string&  
pwl::Version::getBuildDate() const
{
    return mBuildDate;
}

const std::string&  
pwl::Version::getBuildTime() const
{
    return mBuildTime;
}

std::string         
pwl::Version::getStr()
{
    std::string     fullVersionStr  = fmt::sprintf("%s, %d.%d.%d, build at %s %s, id: %s", 
            mProgram,
            mMajorVersion,
            mMinorversion,
            mPatchVersion,
            mBuildTime,
            mBuildDate,
            pwl::Git::getGitCommitHash()
            );

    return fullVersionStr;
}
