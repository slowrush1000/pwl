/**
 * @file pwl_util.hpp
 * @author Cheon Younghoe (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-04-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef PWL_UTIL_H
#define PWL_UTIL_H

#include <cfloat>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace pwl
{
    namespace fs_tt = std::filesystem;
    const std::size_t kBufferSize1k = 1024;

    std::string getCurrentTime();
    std::string getCurrentDir();
    std::string getUser();
    std::string getHostName();
    int getProcessId();
    std::string getOSVersion();
    std::string getCPUInfo();
    void changeStr(std::string &str, const std::string &from,
                   const std::string &to);
    void tokenize(std::string_view str, std::vector<std::string> &tokens,
                  std::string_view delims = " ");
    void setCasesensitive(std::string &str, const bool casesensitive);
    void toUpperStr(std::string &str);
    void toLowerStr(std::string &str);
    bool isEqual(const std::string &str1, const std::string &str2,
                 const bool casesensitive = false);
    bool isEqualCasesensitive(const std::string &str1, const std::string &str2);
    bool isEqualIncasesensitive(const std::string &str1,
                                const std::string &str2);
    bool isEqual(const unsigned char str1, const unsigned char str2,
                 const bool casesensitive = false);
    bool isEqualCasesensitive(const unsigned char str1,
                              const unsigned char str2);
    bool isEqualIncasesensitive(const unsigned char str1,
                                const unsigned char str2);
    bool isEqual(const float f1, const float f2,
                 const float max_rel_diff = FLT_EPSILON);
    bool isEqual(const double d1, const double d2,
                 const double max_rel_diff = DBL_EPSILON);
    std::string makeLine(const std::vector<std::string> &tokens,
                         const std::size_t fromPos, const std::size_t toPos,
                         const std::string &delims = std::string(" "));
    // TODO
    double atof2(const std::string &str);
} // namespace pwl

#endif // PWL_UTIL_H
