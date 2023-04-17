/**
 * @file pwl_log.cpp
 * @author Cheon Younghoe (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-04-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "pwl_log.hpp"
#include <iostream>

pwl::Log::Log()
{
    try
    {
        mConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        mConsoleSink->set_pattern(mLogPattern);
        mConsoleSink->set_level(mLogLevel);

        mFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            mLogFileName, true);
        mFileSink->set_pattern(mLogPattern);
        mFileSink->set_level(mLogLevel);

        spdlog::sinks_init_list sinks = {mConsoleSink, mFileSink};

        mLogger = std::make_shared<spdlog::logger>(mLoggerName, sinks);
        mLogger->set_pattern(mLogPattern);
        mLogger->set_level(mLogLevel);

        spdlog::set_default_logger(mLogger);
    }
    catch (const spdlog::spdlog_ex &ex)
    {
        std::cout << "# error: log init. failed. : " << ex.what() << "\n";
        exit(0);
    }
}

pwl::Log::Log(const std::string &loggerName, const std::string &logFileName)
    : mLoggerName(loggerName), mLogFileName(logFileName)
{
    try
    {
        mConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        mConsoleSink->set_pattern(mLogPattern);
        mConsoleSink->set_level(mLogLevel);

        mFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            mLogFileName, true);
        mFileSink->set_pattern(mLogPattern);
        mFileSink->set_level(mLogLevel);

        spdlog::sinks_init_list sink_lists = {mConsoleSink, mFileSink};

        mLogger = std::make_shared<spdlog::logger>(mLoggerName, sink_lists);
        mLogger->set_pattern(mLogPattern);
        mLogger->set_level(mLogLevel);

        spdlog::set_default_logger(mLogger);
    }
    catch (const spdlog::spdlog_ex &ex)
    {
        std::cout << "# error: log init. failed. : " << ex.what() << "\n";
        exit(0);
    }
}

pwl::Log::~Log() {}

std::shared_ptr<spdlog::logger>
pwl::Log::getDefaultLogger(const std::string &defaultLoggerName)
{
    return spdlog::get(defaultLoggerName);
}

void pwl::Log::setLoggerName(const std::string &loggerName)
{
    mLoggerName = loggerName;
}

const std::string &pwl::Log::getLoggerName() const { return mLoggerName; }

void pwl::Log::setLogFileName(const std::string &logFileName)
{
    mLogFileName = logFileName;
}

const std::string &pwl::Log::getLogFileName() const { return mLogFileName; }

void pwl::Log::changeLogLevel(const spdlog::level::level_enum logLevel)
{
    if ((nullptr != mLogger) && (nullptr != mFileSink) &&
        (nullptr != mConsoleSink))
    {
        mLogLevel = logLevel;

        mConsoleSink->set_level(logLevel);
        mFileSink->set_level(logLevel);
        mLogger->set_level(logLevel);
    }
}
