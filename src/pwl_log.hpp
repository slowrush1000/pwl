/**
 * @file pwl_log.hpp
 * @author Cheon Younghoe (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-04-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef PWL_LOG_H
#define PWL_LOG_H

#include "spdlog/common.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <string>

namespace pwl
{
    const std::string kDefaultLoggerName = std::string("pwl");
    const std::string kLogFileName = std::string("pwl.log");
#ifdef NDEBUG
    const std::string kLogPattern = std::string("%v");
#else
    const std::string kLogPattern = std::string("%+");
#endif // NDEBUG

    class Log
    {
      public:
        Log();
        Log(const std::string &loggerName, const std::string &logFileName);
        virtual ~Log();

        static std::shared_ptr<spdlog::logger> getDefaultLogger(
            const std::string &defaultLoggerName = kDefaultLoggerName);
        void setLoggerName(const std::string &logFileName);
        const std::string &getLoggerName() const;
        void setLogFileName(const std::string &logFileName);
        const std::string &getLogFileName() const;

        void changeLogLevel(const spdlog::level::level_enum logLevel);

        void setTrace() { this->changeLogLevel(spdlog::level::trace); }

        void setDebug() { this->changeLogLevel(spdlog::level::debug); }

        void setInfo() { this->changeLogLevel(spdlog::level::info); }

        void setWarn() { this->changeLogLevel(spdlog::level::warn); }

        void setError() { this->changeLogLevel(spdlog::level::err); }

        void setCritical() { this->changeLogLevel(spdlog::level::critical); }

        void setOff() { this->changeLogLevel(spdlog::level::off); }

      private:
        std::shared_ptr<spdlog::logger> mLogger = nullptr;
        std::string mLoggerName = kDefaultLoggerName;
        std::string mLogFileName = kLogFileName;
        std::string mLogPattern = kLogPattern;
        spdlog::level::level_enum mLogLevel = spdlog::level::info;
        std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> mConsoleSink =
            nullptr;
        std::shared_ptr<spdlog::sinks::basic_file_sink_mt> mFileSink = nullptr;
    };

    using pLog = std::shared_ptr<Log>;
} // namespace pwl

#endif // PWL_LOG_H
