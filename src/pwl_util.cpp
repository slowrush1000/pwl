/**
 * @file pwl_util.cpp
 * @author Cheon Younghoe (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-04-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "pwl_util.hpp"
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <fmt/printf.h>
#include <fstream>
#include <sys/utsname.h>
#include <unistd.h>

std::string
pwl::get_current_time()
{
    auto today     = std::chrono::system_clock::now();
    auto tt        = std::chrono::system_clock::to_time_t(today);
    auto ctime_str = std::string(std::ctime(&tt));

    pwl::change_str(ctime_str, std::string("\n"), std::string(""));

    return ctime_str;
}

std::string
pwl::get_current_dir()
{
    auto currentPath = pwl::fs_tt::current_path();

    return currentPath.native();
}

std::string
pwl::get_user()
{
    char user[pwl::k_buffer_size_1k];

    if (0 == getlogin_r(user, pwl::k_buffer_size_1k))
        return std::string(user);
    else
        return std::string("");
}

std::string
pwl::get_host_name()
{
    char host_name[pwl::k_buffer_size_1k];

    if (0 == gethostname(host_name, pwl::k_buffer_size_1k))
        return std::string(host_name);
    else
        return std::string("");
}

int
pwl::get_process_id()
{
    return getpid();
}

std::string
pwl::get_os_version()
{
    struct utsname buf;

    if (0 != uname(&buf))
    {
        return std::string("");
    }
    else
    {
        return fmt::sprintf("%s %s %s %s %s", buf.sysname, buf.nodename, buf.release, buf.version, buf.machine);
    }
}

std::string
pwl::get_cpu_info()
{
    const std::string k_cpu_info_filename = std::string("/proc/cpuinfo");

    std::ifstream file(k_cpu_info_filename);

    if (false == file.is_open())
    {
        return std::string("");
    }

    std::string line;
    std::string delims = std::string(" \t\r\n");
    std::vector<std::string> tokens;

    std::string model_name = std::string("");
    std::string cpuCores   = std::string("");

    while (std::getline(file, line))
    {
        pwl::tokenize(line, tokens, delims);

        if (true == tokens.empty())
        {
            continue;
        }

        if (std::string("model") == tokens[0])
        {
            if (std::string("name") == tokens[1])
            {
                model_name = tokens[3];

                for (auto i = 4; i < tokens.size(); ++i)
                {
                    model_name += std::string(" ");
                    model_name += tokens[i];
                }
            }
        }
        else if (std::string("cpu") == tokens[0])
        {
            if (std::string("cores") == tokens[1])
            {
                cpuCores = tokens[3];
            }
        }
    }

    file.close();

    auto str = fmt::sprintf("%s, %s cores", model_name, cpuCores);

    return str;
}

void
pwl::change_str(std::string& str, const std::string& from, const std::string& to)
{
    if (true == from.empty())
    {
        return;
    }

    std::size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

void
pwl::tokenize(std::string_view str, std::vector<std::string>& tokens, std::string_view delims)
{
    tokens.clear();

    std::string::size_type last_pos = str.find_first_not_of(delims, 0);
    std::string::size_type pos      = str.find_first_of(delims, last_pos);

    while (std::string::npos != pos || std::string::npos != last_pos)
    {
        tokens.emplace_back(str.substr(last_pos, pos - last_pos));

        last_pos = str.find_first_not_of(delims, pos);
        pos      = str.find_first_of(delims, last_pos);
    }
}

void
pwl::set_casesensitive(std::string& str, const bool casesensitive)
{
    if (false == casesensitive)
        pwl::to_lower_str(str);
}

void
pwl::to_upper_str(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::toupper(c); });
}

void
pwl::to_lower_str(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
}

bool
pwl::is_equal(const std::string& str1, const std::string& str2, const bool casesensitive)
{
    if (true == casesensitive)
        return pwl::is_equal_casesensitive(str1, str2);
    else
        return pwl::is_equal_incasesensitive(str1, str2);
}

bool
pwl::is_equal_casesensitive(const std::string& str1, const std::string& str2)
{
    return str1 == str2;
}

bool
pwl::is_equal_incasesensitive(const std::string& str1, const std::string& str2)
{
    return std::equal(str1.begin(),
                      str1.end(),
                      str2.begin(),
                      str2.end(),
                      [](unsigned char a, unsigned char b) { return std::tolower(a) == std::tolower(b); });
}

bool
pwl::is_equal(const unsigned char str1, const unsigned char str2, const bool casesensitive)
{
    if (true == casesensitive)
        return pwl::is_equal(str1, str2);
    else
        return pwl::is_equal_incasesensitive(str1, str2);
}

bool
pwl::is_equal_casesensitive(const unsigned char str1, const unsigned char str2)
{
    return str1 == str2;
}

bool
pwl::is_equal_incasesensitive(const unsigned char str1, const unsigned char str2)
{
    return tolower(str1) == tolower(str2);
}

bool
pwl::is_equal(const float f1, const float f2, const float max_rel_diff)
{
    float diff    = fabsf(f1 - f2);
    float ff1     = fabsf(f1);
    float ff2     = fabsf(f2);

    float largest = (ff2 > ff1) ? ff2 : ff1;

    if (diff <= (largest * max_rel_diff))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
pwl::is_equal(const double d1, const double d2, const double max_rel_diff)
{
    double diff    = fabs(d1 - d2);
    double fd1     = fabs(d1);
    double fd2     = fabs(d2);

    double largest = (fd2 > fd1) ? fd2 : fd1;

    if (diff <= (largest * max_rel_diff))
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::string
pwl::make_line(const std::vector<std::string>& tokens,
               const std::size_t from_pos,
               const std::size_t to_pos,
               const std::string& delims)
{
    auto line = std::string("");
    for (auto pos = from_pos; pos < to_pos; ++pos)
    {
        line += delims;
        line += tokens[pos];
    }

    return line;
}

// TODO
double
pwl::atof2(const std::string& str)
{
    return std::atof(str.c_str());
}
