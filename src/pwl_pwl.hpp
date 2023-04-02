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

#include "pwl_xy.hpp"
#include <vector>
#include <array>

namespace pwl
{
    class PWL
    {
        public:
            PWL() = default;
            PWL(const PWL& other);
            virtual ~PWL();

            void                    addXY(const XY& xy);
            std::vector<XY>&        getXYs();
            std::pair<xy_tt, xy_tt> getOverlapPeriod(const xy_tt x1, const xy_tt x2, const xy_tt x3, const xy_tt x4, bool& isOverlap);
            bool                    isIn(const xy_tt x, const xy_tt x1, const xy_tt x2);
            xy_tt                   getY(const xy_tt x);
            xy_tt                   getAvg(const xy_tt x1, const xy_tt x2);
            xy_tt                   getRMS(const xy_tt x1, const xy_tt x2);
            xy_tt                   getMax(const xy_tt x1, const xy_tt x2);
            xy_tt                   getMin(const xy_tt x1, const xy_tt x2);
            xy_tt                   getMaxPeakToPeak(const xy_tt x1, const xy_tt x2);

            xy_tt                   getArea(const xy_tt x1, const xy_tt x2, const xy_tt x11, const xy_tt y11, const xy_tt x12, const xy_tt y12);
            xy_tt                   getAreaPow2(const xy_tt x1, const xy_tt x2, const xy_tt x11, const xy_tt y11, const xy_tt x12, const xy_tt y12);

            /*
            xy_tt                   getY(const xy_tt x);
            void                    xshift(const xy_tt xshift);
            void                    yscale(const xy_tt yscale);
            xy_tt                   getYInLine(const xy_tt x, const xy_tt x1, const xy_tt y1, const xy_tt x2, const xy_tt y2);
            xy_tt                   getAvg(const xy_tt x1, const xy_tt x2);
            std::size_t             getLowerBoundPos(const xy_tt x);
            std::size_t             getUpperBoundPos(const xy_tt x);
            xy_tt                   getAreaPow2InLine(const xy_tt x1, const xy_tt y1, const xy_tt x2, const xy_tt y2);
            xy_tt                   getMaxInLine(const xy_tt x1, const xy_tt y1, const xy_tt x2, const xy_tt y2);
            xy_tt                   getMinInLine(const xy_tt x1, const xy_tt y1, const xy_tt x2, const xy_tt y2);
            xy_tt                   getPeakInLine(const xy_tt x1, const xy_tt y1, const xy_tt x2, const xy_tt y2);
            bool                    isPoint(const xy_tt x1, const xy_tt y1, const xy_tt x2, const xy_tt y2);
            bool                    isVerticalLine(const xy_tt x1, const xy_tt y1, const xy_tt x2, const xy_tt y2);
            bool                    isHorizontalLine(const xy_tt x1, const xy_tt y1, const xy_tt x2, const xy_tt y2);
            bool                    isInLine(const xy_tt x, const xy_tt x1, const xy_tt x2);

            PWL&                    operator=(const PWL& other);
            */

        private:
            std::vector<XY>     mXYs;
    };
}

#endif // PWL_PWL_H