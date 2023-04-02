/**
 * @file pwl_pwl.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-04-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "pwl_pwl.hpp"
#include "pwl_util.hpp"
#include <algorithm>

pwl::PWL::PWL(const pwl::PWL& other)
{
    std::copy(other.mXYs.begin(), other.mXYs.end(), std::back_inserter(mXYs));
}

pwl::PWL::~PWL()
{
}

void
pwl::PWL::addXY(const pwl::XY& xy)
{
    mXYs.emplace_back(xy);
}

std::vector<pwl::XY>&
pwl::PWL::getXYs()
{
    return mXYs;
}

// case 1 : no overlap
//         x1-x2
// x3-x4
//                 x3---x4
// case 2 : include
// x1--------------------------x2
//        x3--------------x4
// case 3 : left overlap
//       x1-------------x2
// x3----------x4
// case 3 : right overlap
// x1-------------x2
//           x3----------x4
std::pair<pwl::xy_tt, pwl::xy_tt>
pwl::PWL::getOverlapPeriod(const pwl::xy_tt x1, const pwl::xy_tt x2, const pwl::xy_tt x3, const pwl::xy_tt x4, bool& isOverlap)
{
    auto    x31 = x3;
    auto    x41 = x4;

    // case 1 : no overlap
    if ((x4 < x1) || (x2 < x3))
    {
        isOverlap   = false;
        x31         = x3;
        x41         = x4;
    }
    else
    {
        isOverlap   = true;

        // case 2 : include
        if ((x1 <= x3) && (x4 <= x2))
        {
            x31         = x3;
            x41         = x4;
        }
        else
        {
            // case 3 : left overlap
            if ((x3 < x1) && (true == this->isIn(x4, x1, x2)))
            {
                x31     = x1;
                x41     = x4;
            }
            // case 4 : right overlap
            else if ((true == this->isIn(x3, x1, x2)) && (x2 < x4))
            {
                x31     = x3;
                x41     = x2;
            }
        }
    }

    return std::pair<pwl::xy_tt, pwl::xy_tt>(x31, x41);
}

bool
pwl::PWL::isIn(const pwl::xy_tt x, const pwl::xy_tt x1, const pwl::xy_tt x2)
{
    if ((x1 <= x) && (x <= x2))
    {
        return true;
    }
    else
    {
        return false;
    }
}

pwl::xy_tt
pwl::PWL::getY(const pwl::xy_tt x)
{
    for (auto pos = 0; pos < (mXYs.size() - 1); ++pos)
    {
        auto    x1  = mXYs[pos].getX();
        auto    y1  = mXYs[pos].getY();
        auto    x2  = mXYs[pos + 1].getX();
        auto    y2  = mXYs[pos + 1].getY();

        if (true == this->isIn(x, x1, x2))
        {
            auto    a   = (y2 - y1) / (x2 - x1);
            auto    b   = y1 - a * x1;

            return a * x + b;
        }
    }

    return kXYttNaN;
}

pwl::xy_tt
pwl::PWL::getAvg(const pwl::xy_tt x1, const pwl::xy_tt x2)
{
    auto    sum     = pwl::kXYttInit;

    for (auto pos = 0; pos < (mXYs.size() - 1); ++pos)
    {
        auto    x11  = mXYs[pos].getX();
        auto    y11  = mXYs[pos].getY();
        auto    x12  = mXYs[pos + 1].getX();
        auto    y12  = mXYs[pos + 1].getY();

        bool    isOverlap   = false;
        auto    xy22        = this->getOverlapPeriod(x1, x2, x11, x12, isOverlap);

        if (true == isOverlap)
        {
            sum     += this->getArea(xy22.first, xy22.second, x11, y11, x12, y12);
        }
    }

    return sum / (x2 - x1);
}

pwl::xy_tt
pwl::PWL::getRMS(const pwl::xy_tt x1, const pwl::xy_tt x2)
{
    auto    sum     = pwl::kXYttInit;

    for (auto pos = 0; pos < (mXYs.size() - 1); ++pos)
    {
        auto    x11  = mXYs[pos].getX();
        auto    y11  = mXYs[pos].getY();
        auto    x12  = mXYs[pos + 1].getX();
        auto    y12  = mXYs[pos + 1].getY();

        bool    isOverlap   = false;
        auto    xy22        = this->getOverlapPeriod(x1, x2, x11, x12, isOverlap);

        if (true == isOverlap)
        {
            sum     += this->getAreaPow2(xy22.first, xy22.second, x11, y11, x12, y12);
        }
    }

    return std::sqrt(sum / (x2 - x1));
}

pwl::xy_tt
pwl::PWL::getMax(const pwl::xy_tt x1, const pwl::xy_tt x2)
{
    auto    max     = pwl::kXYttMin;

    for (auto pos = 0; pos < (mXYs.size() - 1); ++pos)
    {
        auto    x11  = mXYs[pos].getX();
        auto    y11  = mXYs[pos].getY();
        auto    x12  = mXYs[pos + 1].getX();
        auto    y12  = mXYs[pos + 1].getY();

        bool    isOverlap   = false;
        auto    xy22        = this->getOverlapPeriod(x1, x2, x11, x12, isOverlap);

        if (true == isOverlap)
        {
            auto    xy22y1      = this->getY(xy22.first);
            auto    xy22y2      = this->getY(xy22.second);

            max     = std::max<pwl::xy_tt>(max, std::max<pwl::xy_tt>(xy22y1, xy22y2));
        }
    }

    return max;
}

pwl::xy_tt
pwl::PWL::getMin(const pwl::xy_tt x1, const pwl::xy_tt x2)
{
    auto    min     = pwl::kXYttMax;

    for (auto pos = 0; pos < (mXYs.size() - 1); ++pos)
    {
        auto    x11  = mXYs[pos].getX();
        auto    y11  = mXYs[pos].getY();
        auto    x12  = mXYs[pos + 1].getX();
        auto    y12  = mXYs[pos + 1].getY();

        bool    isOverlap   = false;
        auto    xy22        = this->getOverlapPeriod(x1, x2, x11, x12, isOverlap);

        if (true == isOverlap)
        {
            auto    xy22y1      = this->getY(xy22.first);
            auto    xy22y2      = this->getY(xy22.second);

            min     = std::min<pwl::xy_tt>(min, std::min<pwl::xy_tt>(xy22y1, xy22y2));
        }
    }

    return min;
}

pwl::xy_tt
pwl::PWL::getMaxPeakToPeak(const pwl::xy_tt x1, const pwl::xy_tt x2)
{
    auto    maxPeakToPeak   = pwl::kXYttMin;

    for (auto pos = 0; pos < (mXYs.size() - 1); ++pos)
    {
        auto    x11  = mXYs[pos].getX();
        auto    y11  = mXYs[pos].getY();
        auto    x12  = mXYs[pos + 1].getX();
        auto    y12  = mXYs[pos + 1].getY();

        bool    isOverlap   = false;
        auto    xy22        = this->getOverlapPeriod(x1, x2, x11, x12, isOverlap);

        if (true == isOverlap)
        {
            auto    xy22y1      = this->getY(xy22.first);
            auto    xy22y2      = this->getY(xy22.second);

            auto    peakToPeak  = std::max<pwl::xy_tt>(xy22y1, xy22y2) - std::min<pwl::xy_tt>(xy22y1, xy22y2);

            maxPeakToPeak       = std::max<pwl::xy_tt>(maxPeakToPeak, peakToPeak);
        }
    }

    return maxPeakToPeak;
}

pwl::xy_tt
pwl::PWL::getArea(const pwl::xy_tt x1, const pwl::xy_tt x2, const pwl::xy_tt x11, const pwl::xy_tt y11, const pwl::xy_tt x12, const pwl::xy_tt y12)
{
    auto    a = (y12 - y11) / (x12 - x11);
    auto    b = y11 - a * x11;

    return a * (x2 * x2 - x1 * x1) / 2.0 + b * (x2 - x1);
}

pwl::xy_tt
pwl::PWL::getAreaPow2(const pwl::xy_tt x1, const pwl::xy_tt x2, const pwl::xy_tt x11, const pwl::xy_tt y11, const pwl::xy_tt x12, const pwl::xy_tt y12)
{
    auto    a = (y12 - y11) / (x12 - x11);
    auto    b = y11 - a * x11;

    return  a * a * (x2 * x2 * x2 - x1 * x1 * x1) / 3.0 + a * b * (x2 * x2 - x1 * x1) + b * b * (x2 - x1);
}

/*
std::array<pwl::xy_tt, 5>
pwl::PWL::getAvgRMSMaxMinPeak(const pwl::xy_tt x1, const pwl::xy_tt x2)
{
    std::array<pwl::xy_tt, 5>   avgRMSMaxMinPeak;

    return avgRMSMaxMinPeak;
}

// satify element < value
std::size_t
pwl::PWL::getLowerBoundPos(const pwl::xy_tt x)
{
    auto    lowerBound  = std::lower_bound(mXYs.begin(), mXYs.end(), pwl::XY(x, 0.0));

    if (mXYs.end() == lowerBound)
    {
        return mXYs.size();
    }
    else
    {
        return std::distance(mXYs.begin(), lowerBound);
    }
}

// satify element > value
std::size_t
pwl::PWL::getUpperBoundPos(const pwl::xy_tt x)
{
    auto    upperBound  = std::upper_bound(mXYs.begin(), mXYs.end(), pwl::XY(x, 0.0));

    if (mXYs.end() == upperBound)
    {
        return mXYs.size();
    }
    else
    {
        return std::distance(mXYs.begin(), upperBound);
    }
}

void
pwl::PWL::xshift(const pwl::xy_tt xshift)
{
    std::for_each(mXYs.begin(), mXYs.end(), [xshift](pwl::XY & xy)
    {
        xy.setX(xy.getX() - xshift);
    }
                 );
}

void
pwl::PWL::yscale(const pwl::xy_tt yscale)
{
    std::for_each(mXYs.begin(), mXYs.end(), [yscale](pwl::XY & xy)
    {
        xy.setY(xy.getX()*yscale);
    }
                 );
}

pwl::xy_tt
pwl::PWL::getYInLine(const pwl::xy_tt x, const pwl::xy_tt x1, const pwl::xy_tt y1, const pwl::xy_tt x2, const pwl::xy_tt y2)
{
    if (true == pwl::isEqual(x, x1))
    {
        return y1;
    }

    if (true == pwl::isEqual(x, x2))
    {
        return y2;
    }

    return (y2 - y1) * (x - x1) / (x2 - x1) + y1;
}

pwl::xy_tt
pwl::PWL::getAvg(const pwl::xy_tt x1, const pwl::xy_tt x2)
{
    auto    x1Pos   = this->getLowerBoundPos(x1);
    auto    x2Pos   = this->getUpperBoundPos(x2);

    for (auto pos = x1Pos; pos <= x2Pos; ++pos)
    {
        if (x1Pos == pos)
        {
        }
        else if (x2Pos == pos)
        {

        }
        else
        {

        }
    }
}


pwl::xy_tt
pwl::PWL::getAreaInLine(const pwl::xy_tt x1, const pwl::xy_tt y1, const pwl::xy_tt x2, const pwl::xy_tt y2)
{
    // |
    // vertical line은 없다고 가정한다.

    // -
    if (y1 == y2)
    {
        return y1;
    }

    //
    return 0.5 * (x2 - x1) * (y1 + y2);
}

pwl::xy_tt
pwl::PWL::getAreaPow2InLine(const pwl::xy_tt x1, const pwl::xy_tt y1, const pwl::xy_tt x2, const pwl::xy_tt y2)
{
    // |
    // vertical line은 없다고 가정한다.

    // -
    if (y1 == y2)
    {
        return std::fabs(y1);
    }

    //
    xy_tt   a   = (y2 - y1) / (x2 - x1);
    xy_tt   b   = y1 - a * x1;

    return std::pow(a, 3.0) * (std::pow(x2, 3.0) - std::pow(x1, 3.0)) / 3.0 + a * b * (x2 * x2 - x1 * x1) + b * b * (x2 - x1);
}

pwl::xy_tt
pwl::PWL::getMaxInLine(const pwl::xy_tt x1, const pwl::xy_tt y1, const pwl::xy_tt x2, const pwl::xy_tt y2)
{
    return std::max<pwl::xy_tt>(y1, y2);
}

pwl::xy_tt
pwl::PWL::getMinInLine(const pwl::xy_tt x1, const pwl::xy_tt y1, const pwl::xy_tt x2, const pwl::xy_tt y2)
{
    return std::min<pwl::xy_tt>(y1, y2);
}

pwl::xy_tt
pwl::PWL::getPeakInLine(const pwl::xy_tt x1, const pwl::xy_tt y1, const pwl::xy_tt x2, const pwl::xy_tt y2)
{
    return std::max<pwl::xy_tt>(y1, y2) - std::min<pwl::xy_tt>(y1, y2);
}

bool
pwl::PWL::isPoint(const pwl::xy_tt x1, const pwl::xy_tt y1, const pwl::xy_tt x2, const pwl::xy_tt y2)
{
    if ((true == pwl::isEqual(x1, x2)) && (true == pwl::isEqual(y1, y2)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
pwl::PWL::isVerticalLine(const pwl::xy_tt x1, const pwl::xy_tt y1, const pwl::xy_tt x2, const pwl::xy_tt y2)
{
    if (true == pwl::isEqual(x1, x2))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
pwl::PWL::isHorizontalLine(const pwl::xy_tt x1, const pwl::xy_tt y1, const pwl::xy_tt x2, const pwl::xy_tt y2)
{
    if (true == pwl::isEqual(y1, y2))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
pwl::PWL::isInLine(const pwl::xy_tt x, const pwl::xy_tt x1, const pwl::xy_tt x2)
{
}

pwl::PWL&
pwl::PWL::operator=(const pwl::PWL& other)
{
    if (this == &other)
    {
        return *this;
    }

    std::copy(other.mXYs.begin(), other.mXYs.end(), std::back_inserter(mXYs));

    return *this;
}
*/