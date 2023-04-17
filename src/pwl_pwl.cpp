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
#include <fmt/printf.h>
#include <fstream>
#include <gzstream.h>

pwl::PWL::PWL(const pwl::PWL& other)
{
    std::copy(other.m_x.begin(), other.m_x.end(), std::back_inserter(m_x));
    std::copy(other.m_y.begin(), other.m_y.end(), std::back_inserter(m_y));
}

pwl::PWL::~PWL()
{
}

void
pwl::PWL::add(const pwl::xy_tt x, const pwl::xy_tt y)
{
    m_x.emplace_back(x);
    m_y.emplace_back(y);
}

std::vector<pwl::xy_tt>&
pwl::PWL::x()
{
    return m_x;
}

std::vector<pwl::xy_tt>&
pwl::PWL::y()
{
    return m_y;
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
pwl::PWL::get_overlap_period(const pwl::xy_tt x1, const pwl::xy_tt x2, const pwl::xy_tt x3, const pwl::xy_tt x4)
{
    auto x_start = x3;
    auto x_end   = x4;

    // case 1 : no overlap
    if ((x4 < x1) || (x2 < x3))
    {
        x_start = pwl::k_xy_tt_max;
        x_end   = pwl::k_xy_tt_min;
    }
    else
    {
        // case 2 : include
        if ((x1 <= x3) && (x4 <= x2))
        {
            x_start = x3;
            x_end   = x4;
        }
        else
        {
            // case 3 : left overlap
            if ((x3 < x1) && (true == this->is_in(x4, x1, x2)))
            {
                x_start = x1;
                x_end   = x4;
            }
            // case 4 : right overlap
            else if ((true == this->is_in(x3, x1, x2)) && (x2 < x4))
            {
                x_start = x3;
                x_end   = x2;
            }
        }
    }

    return std::pair<pwl::xy_tt, pwl::xy_tt>(x_start, x_end);
}

bool
pwl::PWL::is_in(const pwl::xy_tt x, const pwl::xy_tt x1, const pwl::xy_tt x2)
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
pwl::PWL::y(const pwl::xy_tt x)
{
    for (auto pos = 0; pos < (m_x.size() - 1); ++pos)
    {
        auto x1 = m_x[pos];
        auto y1 = m_y[pos];
        auto x2 = m_x[pos + 1];
        auto y2 = m_y[pos + 1];

        if (true == this->is_in(x, x1, x2))
        {
            auto a = (y2 - y1) / (x2 - x1);
            auto b = y1 - a * x1;

            return a * x + b;
        }
    }

    return pwl::k_xy_tt_nan;
}

pwl::xy_tt
pwl::PWL::avg(const pwl::xy_tt x1, const pwl::xy_tt x2)
{
    auto sum = pwl::k_xy_tt_init;

    for (auto pos = 0; pos < (m_x.size() - 1); ++pos)
    {
        auto x11  = m_x[pos];
        auto y11  = m_y[pos];
        auto x12  = m_x[pos + 1];
        auto y12  = m_y[pos + 1];

        auto xy22 = this->get_overlap_period(x1, x2, x11, x12);

        if (xy22.first > xy22.second)
        {
            continue;
        }

        sum += this->area(xy22.first, xy22.second, x11, y11, x12, y12);
    }

    return sum / (x2 - x1);
}

pwl::xy_tt
pwl::PWL::rms(const pwl::xy_tt x1, const pwl::xy_tt x2)
{
    auto sum = pwl::k_xy_tt_init;

    for (auto pos = 0; pos < (m_x.size() - 1); ++pos)
    {
        auto x11  = m_x[pos];
        auto y11  = m_y[pos];
        auto x12  = m_x[pos + 1];
        auto y12  = m_y[pos + 1];

        auto xy22 = this->get_overlap_period(x1, x2, x11, x12);

        if (xy22.first > xy22.second)
        {
            continue;
        }

        sum += this->area_pow_2(xy22.first, xy22.second, x11, y11, x12, y12);
    }

    return std::sqrt(sum / (x2 - x1));
}

pwl::xy_tt
pwl::PWL::max(const pwl::xy_tt x1, const pwl::xy_tt x2)
{
    auto max = pwl::k_xy_tt_min;

    for (auto pos = 0; pos < (m_x.size() - 1); ++pos)
    {
        auto x11  = m_x[pos];
        auto y11  = m_y[pos];
        auto x12  = m_x[pos + 1];
        auto y12  = m_y[pos + 1];

        auto xy22 = this->get_overlap_period(x1, x2, x11, x12);

        if (xy22.first > xy22.second)
        {
            continue;
        }

        auto xy22y1 = this->y(xy22.first);
        auto xy22y2 = this->y(xy22.second);
        max         = std::max({max, xy22y1, xy22y2});
    }

    return max;
}

pwl::xy_tt
pwl::PWL::min(const pwl::xy_tt x1, const pwl::xy_tt x2)
{
    auto min = pwl::k_xy_tt_max;

    for (auto pos = 0; pos < (m_x.size() - 1); ++pos)
    {
        auto x11  = m_x[pos];
        auto y11  = m_y[pos];
        auto x12  = m_x[pos + 1];
        auto y12  = m_y[pos + 1];

        auto xy22 = this->get_overlap_period(x1, x2, x11, x12);

        if (xy22.first > xy22.second)
        {
            continue;
        }

        auto xy22y1 = this->y(xy22.first);
        auto xy22y2 = this->y(xy22.second);
        min         = std::min({min, xy22y1, xy22y2});
    }

    return min;
}

pwl::xy_tt
pwl::PWL::peak_to_peak(const pwl::xy_tt x1, const pwl::xy_tt x2)
{
    return this->max(x1, x2) - this->min(x1, x2);
}

void
pwl::PWL::xshift(const pwl::xy_tt xshift)
{
    for (auto pos = 0; pos < m_x.size(); ++pos)
    {
        m_x[pos] += xshift;
    }
}

void
pwl::PWL::xscale(const pwl::xy_tt xscale)
{
    for (auto pos = 0; pos < m_x.size(); ++pos)
    {
        m_x[pos] *= xscale;
    }
}

void
pwl::PWL::yscale(const pwl::xy_tt yscale)
{
    for (auto pos = 0; pos < m_x.size(); ++pos)
    {
        m_y[pos] *= yscale;
    }
}

pwl::xy_tt
pwl::PWL::area(const pwl::xy_tt x1,
               const pwl::xy_tt x2,
               const pwl::xy_tt x11,
               const pwl::xy_tt y11,
               const pwl::xy_tt x12,
               const pwl::xy_tt y12)
{
    auto a = (y12 - y11) / (x12 - x11);
    auto b = y11 - a * x11;

    return a * (x2 * x2 - x1 * x1) / 2.0 + b * (x2 - x1);
}

pwl::xy_tt
pwl::PWL::area_pow_2(const pwl::xy_tt x1,
                     const pwl::xy_tt x2,
                     const pwl::xy_tt x11,
                     const pwl::xy_tt y11,
                     const pwl::xy_tt x12,
                     const pwl::xy_tt y12)
{
    auto a = (y12 - y11) / (x12 - x11);
    auto b = y11 - a * x11;

    return a * a * (x2 * x2 * x2 - x1 * x1 * x1) / 3.0 + a * b * (x2 * x2 - x1 * x1) + b * b * (x2 - x1);
}

/*
void
pwl::PWL::add(pwl::PWL& other)
{
    auto xs1 = this->getXs();
    auto xs2 = other.getXs();

    auto xs  = this->mergeXs(xs1, xs2);
    auto ys  = std::vector<pwl::xy_tt>(xs.size());

    for (auto pos = 0; pos < xs.size(); ++pos)
    {
        auto y1 = this->getY(xs[pos]);
        auto y2 = other.getY(xs[pos]);

        ys[pos] = y1 + y2;
    }

    mXYs.resize(xs.size());

    for (auto pos = 0; pos < xs.size(); ++pos)
    {
        mXYs[pos] = pwl::XY(xs[pos], ys[pos]);
    }
}

std::vector<pwl::xy_tt>
pwl::PWL::mergeXs(const std::vector<pwl::xy_tt>& xs1, const std::vector<pwl::xy_tt>& xs2)
{
    std::vector<pwl::xy_tt> xs;

    for (auto pos = 0; pos < xs1.size(); ++pos)
    {
        xs.emplace_back(xs1[pos]);
    }

    for (auto pos = 0; pos < xs2.size(); ++pos)
    {
        xs.emplace_back(xs2[pos]);
    }

    std::sort(xs.begin(), xs.end());
    auto last = std::unique(xs.begin(), xs.end());
    xs.erase(last, xs.end());

    return xs;
}
*/

pwl::PWL&
pwl::PWL::operator=(const pwl::PWL& other)
{
    if (this == &other)
    {
        return *this;
    }

    std::copy(other.m_x.begin(), other.m_x.end(), std::back_inserter(m_x));
    std::copy(other.m_y.begin(), other.m_y.end(), std::back_inserter(m_y));

    return *this;
}

// file format : index time value
void
pwl::PWL::read_file(const std::string& fileName, const std::size_t xPos, const std::size_t yPos)
{
    igzstream file;
    file.open(fileName.c_str());

    if (false == file.good())
    {
        std::cout << "# error : file open failed.\n";
        return;
    }

    std::vector<std::string> tokens;
    std::string delims(" \t\r\n");
    std::string line;
    std::size_t nLines = 0;

    while (std::getline(file, line))
    {
        pwl::tokenize(line, tokens, delims);

        if (true == tokens.empty())
        {
            continue;
        }

        auto x = pwl::atof2(tokens[xPos]);
        auto y = pwl::atof2(tokens[yPos]);

        this->add(x, y);
    }

    file.close();
}

void
pwl::PWL::write_file(const std::string& fileName)
{
    std::ofstream file(fileName);

    if (false == file.is_open())
    {
        std::cout << "# error : file open failed.\n";
        return;
    }

    for (auto pos = 0; pos < m_x.size(); ++pos)
    {
        file << fmt::sprintf("%lld %e %e\n", pos, m_x[pos], m_y[pos]);
    }

    file.close();
}