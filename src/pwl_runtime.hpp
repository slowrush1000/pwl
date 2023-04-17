/**
 * @file pwl_runtime.hpp
 * @author Cheon Younghoe (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-04-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef PWL_RUNTIME_H
#define PWL_RUNTIME_H

#include <chrono>
#include <memory>

namespace pwl {
class Runtime {
public:
  Runtime();
  virtual ~Runtime();

  double getCPUTimeSecs();
  double getWallTimeSecs();

  std::size_t getPeakRSSBytes();
  std::size_t getCurrentRSSBytes();

  std::string getRuntimeStr();

private:
  std::clock_t mCPUTime;
  std::chrono::time_point<std::chrono::high_resolution_clock> mWallTime;
};

using pRuntime = std::shared_ptr<Runtime>;
} // namespace pwl

#endif // PWL_RUNTIME_H
