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
#include <initializer_list>
#include <string>

namespace pwl
{
    class PWL
    {
        public:
            PWL() = default;
            PWL(std::initializer_list<XY> list);
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

            void                    xshift(const xy_tt xshift);
            void                    yscale(const xy_tt yscale);
            void                    add(PWL& other);

            std::vector<xy_tt>      getXs();
            std::vector<xy_tt>      getYs();
            std::vector<xy_tt>      mergeXs(const std::vector<xy_tt>& xs1, const std::vector<xy_tt>& xs2);

            xy_tt                   getArea(const xy_tt x1, const xy_tt x2, const xy_tt x11, const xy_tt y11, const xy_tt x12, const xy_tt y12);
            xy_tt                   getAreaPow2(const xy_tt x1, const xy_tt x2, const xy_tt x11, const xy_tt y11, const xy_tt x12, const xy_tt y12);

            PWL&                    operator=(const PWL& other);

            void                    readFile(const std::string& fileName, const std::size_t xPos, const std::size_t yPos);
            void                    writeFile(const std::string& fileName);

        private:
            std::vector<XY>        mXYs;
    };
}

#endif // PWL_PWL_H