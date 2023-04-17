/**
 * @file pwl_xy.hpp
 * @author Cheon Younghoe (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-04-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef PWL_XY_H
#define PWL_XY_H

#include <cmath>
#include <initializer_list>
#include <limits>

namespace pwl {

#define DATATYPE_DOUBLE
#if defined(DATATYPE_DOUBLE)
using xy_tt = double;
const xy_tt kXYttNaN = std::nan("");
#else
using xy_tt = float;
const xy_tt kXYttNaN = xy_tt(std::nan(""));
#endif

constexpr xy_tt kXYttInit = 0.0;
constexpr xy_tt kXYttMax = std::numeric_limits<xy_tt>::max();
constexpr xy_tt kXYttMin = std::numeric_limits<xy_tt>::lowest();

class XY {
public:
  XY() = default;
  XY(const xy_tt x, const xy_tt y);
  XY(std::initializer_list<xy_tt> l);
  XY(const XY &other);
  ~XY();

  void setX(const xy_tt x);
  xy_tt getX() const;
  void setY(const xy_tt y);
  xy_tt getY() const;

  XY &operator=(const XY &other);
  bool operator<(const XY &other) const;

private:
  xy_tt mX = kXYttInit;
  xy_tt mY = kXYttInit;
};
} // namespace pwl

#endif // PWL_XY_H
