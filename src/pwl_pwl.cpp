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
#include <fstream>
#include <gzstream.h>
#include <fmt/printf.h>

pwl::PWL::PWL(const pwl::PWL& other)
{
    std::copy(other.mXYs.begin(), other.mXYs.end(), std::back_inserter(mXYs));
}

pwl::PWL::PWL(std::initializer_list<pwl::XY> list) : mXYs(list)
{
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

    return pwl::kXYttInit;
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
    auto    maxPeakToPeak   = this->getMax(x1, x2) - this->getMin(x1, x2);

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

void
pwl::PWL::xshift(const pwl::xy_tt xshift)
{
    for (auto pos = 0; pos < mXYs.size(); ++pos)
    {
        auto    x   = mXYs[pos].getX();
        mXYs[pos].setX(x + xshift);
    }
}

void
pwl::PWL::yscale(const pwl::xy_tt yscale)
{
    for (auto pos = 0; pos < mXYs.size(); ++pos)
    {
        auto    y   = mXYs[pos].getY();

        mXYs[pos].setY(y * yscale);
    }
}

void
pwl::PWL::add(pwl::PWL& other)
{
    auto    xs1     = this->getXs();
    auto    xs2     = other.getXs();

    auto    xs      = this->mergeXs(xs1, xs2);
    auto    ys      = std::vector<pwl::xy_tt>(xs.size());

    for (auto pos = 0; pos < xs.size(); ++pos)
    {
        auto    y1  = this->getY(xs[pos]);
        auto    y2  = other.getY(xs[pos]);

        ys[pos]     = y1 + y2;
    }

    mXYs.resize(xs.size());

    for (auto pos = 0; pos < xs.size(); ++pos)
    {
        mXYs[pos]   = pwl::XY(xs[pos], ys[pos]);
    }
}

std::vector<pwl::xy_tt>
pwl::PWL::getXs()
{
    std::vector<pwl::xy_tt>     xs;

    for (auto pos = 0; pos < mXYs.size(); ++pos)
    {
        xs.emplace_back(mXYs[pos].getX());
    }

    return xs;
}

std::vector<pwl::xy_tt>
pwl::PWL::getYs()
{
    std::vector<pwl::xy_tt>     ys;

    for (auto pos = 0; pos < mXYs.size(); ++pos)
    {
        ys.emplace_back(mXYs[pos].getY());
    }

    return ys;
}

std::vector<pwl::xy_tt>
pwl::PWL::mergeXs(const std::vector<pwl::xy_tt>& xs1, const std::vector<pwl::xy_tt>& xs2)
{
    std::vector<pwl::xy_tt>     xs;

    for (auto pos = 0; pos < xs1.size(); ++pos)
    {
        xs.emplace_back(xs1[pos]);
    }

    for (auto pos = 0; pos < xs2.size(); ++pos)
    {
        xs.emplace_back(xs2[pos]);
    }

    std::sort(xs.begin(), xs.end());
    auto last   = std::unique(xs.begin(), xs.end());
    xs.erase(last, xs.end());

    return xs;
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

// file format : index time value
void
pwl::PWL::readFile(const std::string& fileName, const std::size_t xPos, const std::size_t yPos)
{
    igzstream   file;
    file.open(fileName.c_str());

    if (false == file.good())
    {
        std::cout << "# error : file open failed.\n";
        return;
    }

    std::vector<std::string>    tokens;
    std::string                 delims(" \t\r\n");
    std::string                 line;
    std::size_t                 nLines  = 0;

    while (std::getline(file, line))
    {
        pwl::tokenize(line, tokens, delims);

        if (true == tokens.empty())
        {
            continue;
        }

        auto    x               = pwl::atof2(tokens[xPos]);
        auto    y               = pwl::atof2(tokens[yPos]);

        this->addXY(pwl::XY(x, y));
    }

    file.close();
}

void
pwl::PWL::writeFile(const std::string& fileName)
{
    std::ofstream   file(fileName);

    if (false == file.is_open())
    {
        std::cout << "# error : file open failed.\n";
        return;
    }

    for (auto index = 0; index < mXYs.size(); ++index)
    {
        file << fmt::sprintf("%lld %e %e\n", index, mXYs[index].getX(), mXYs[index].getY());
    }

    file.close();
}