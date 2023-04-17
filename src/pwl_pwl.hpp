/**
 * @file pwl_pwl.hpp
 * @author Cheon Younghoe (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-04-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef PWL_PWL_H
#define PWL_PWL_H

// include "pwl_xy.hpp"
#include <array>
#include <cmath>
#include <initializer_list>
#include <limits>
#include <memory>
#include <string>
#include <vector>

namespace pwl
{
#define XY_TT_DOUBLE
#if defined(XY_TT_DOUBLE)
    using xy_tt             = double;
    const xy_tt k_xy_tt_nan = std::nan("1");
#else
    using xy_tt             = float;
    const xy_tt k_xy_tt_nan = std::nan("1");
#endif

    constexpr xy_tt k_xy_tt_init = 0.0;
    constexpr xy_tt k_xy_tt_max  = std::numeric_limits<xy_tt>::max();
    constexpr xy_tt k_xy_tt_min  = std::numeric_limits<xy_tt>::lowest();

    class PWL;
    using pPWL = std::shared_ptr<PWL>;

    class PWL
    {
        public:
            PWL() = default;
            PWL(const PWL& other);
            virtual ~PWL();

            void add(const xy_tt x, const xy_tt y);
            std::vector<xy_tt>& x();
            std::vector<xy_tt>& y();
            std::pair<xy_tt, xy_tt> get_overlap_period(const xy_tt x1, const xy_tt x2, const xy_tt x3, const xy_tt x4);
            bool is_in(const xy_tt x, const xy_tt x1, const xy_tt x2);
            xy_tt y(const xy_tt x);

            xy_tt avg(const xy_tt x1, const xy_tt x2);
            xy_tt rms(const xy_tt x1, const xy_tt x2);
            xy_tt max(const xy_tt x1, const xy_tt x2);
            xy_tt min(const xy_tt x1, const xy_tt x2);
            xy_tt peak_to_peak(const xy_tt x1, const xy_tt x2);

            void xshift(const xy_tt xshift);
            void xscale(const xy_tt xscale);
            void yscale(const xy_tt yscale);

            xy_tt area(
                const xy_tt x1, const xy_tt x2, const xy_tt x11, const xy_tt y11, const xy_tt x12, const xy_tt y12);
            xy_tt area_pow_2(
                const xy_tt x1, const xy_tt x2, const xy_tt x11, const xy_tt y11, const xy_tt x12, const xy_tt y12);

            /*
            void add(PWL& other);

            std::vector<xy_tt> getXs();
            std::vector<xy_tt> getYs();
            std::vector<xy_tt> mergeXs(const std::vector<xy_tt>& xs1, const std::vector<xy_tt>& xs2);
            */

            PWL& operator=(const PWL& other);

            void read_file(const std::string& fileName, const std::size_t xPos, const std::size_t yPos);
            void write_file(const std::string& fileName);

        private:
            std::vector<xy_tt> m_x;
            std::vector<xy_tt> m_y;
    };
} // namespace pwl

#endif // PWL_PWL_H