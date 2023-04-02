/**
 * @file pwl_xy.cpp
 * @author Cheon Younghoe (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-04-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "pwl_xy.hpp"

pwl::XY::XY(const pwl::xy_tt x, const pwl::xy_tt y) : mX(x), mY(y)
{
}

pwl::XY::XY(std::initializer_list<pwl::xy_tt> list)
{
    if (2 == list.size())
    {
        auto iter   = list.begin();
        mX          = *iter++;
        mY          = *iter;
    }
}

pwl::XY::XY(const pwl::XY& other) : mX(other.mX), mY(other.mY)
{
}

pwl::XY::~XY()
{
}

void
pwl::XY::setX(const pwl::xy_tt x)
{
    mX = x;
}

pwl::xy_tt
pwl::XY::getX() const
{
    return mX;
}

void
pwl::XY::setY(const pwl::xy_tt y)
{
    mY = y;
}

pwl::xy_tt
pwl::XY::getY() const
{
    return mY;
}

pwl::XY&
pwl::XY::operator=(const pwl::XY& other)
{
    if (this == &other)
    {
        return *this;
    }

    mX  = other.mX;
    mY  = other.mY;

    return *this;
}

bool
pwl::XY::operator<(const pwl::XY& other) const
{
    return mX < other.mX;
}