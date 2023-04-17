/**
 * @file pwl_log.hpp
 * @author Cheon Younghoe (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-04-17
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
    const std::string k_default_logger_name = std::string("pwl");
    const std::string k_log_filename        = std::string("pwl.log");
#ifdef NDEBUG
    const std::string k_log_pattern = std::string("%v");
#else
    const std::string k_log_pattern = std::string("%+");
#endif // NDEBUG

    class Log;
    using pLog = std::shared_ptr<Log>;

    class Log
    {
        public:
            Log();
            Log(const std::string& logger_name, const std::string& log_filename);
            virtual ~Log();

            static std::shared_ptr<spdlog::logger> default_logger(
                const std::string& default_logger_name = k_default_logger_name);
            void set_logger_name(const std::string& log_filename);
            const std::string& logger_name() const;
            void set_log_filename(const std::string& log_filename);
            const std::string& log_filename() const;

            void change_log_level(const spdlog::level::level_enum log_level);

            void
            set_trace()
            {
                this->change_log_level(spdlog::level::trace);
            }
            void
            set_debug()
            {
                this->change_log_level(spdlog::level::debug);
            }
            void
            set_info()
            {
                this->change_log_level(spdlog::level::info);
            }
            void
            set_warn()
            {
                this->change_log_level(spdlog::level::warn);
            }
            void
            set_error()
            {
                this->change_log_level(spdlog::level::err);
            }
            void
            set_critical()
            {
                this->change_log_level(spdlog::level::critical);
            }
            void
            set_off()
            {
                this->change_log_level(spdlog::level::off);
            }

        private:
            std::shared_ptr<spdlog::logger> m_logger                            = nullptr;
            std::string m_logger_name                                           = k_default_logger_name;
            std::string m_log_filename                                          = k_log_filename;
            std::string m_log_pattern                                           = k_log_pattern;
            spdlog::level::level_enum m_log_level                               = spdlog::level::info;
            std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> m_console_sink = nullptr;
            std::shared_ptr<spdlog::sinks::basic_file_sink_mt> m_file_sink      = nullptr;
    };
} // namespace pwl
#endif // PWL_LOG_H