/****************************************************************************
  Copyright (c) 2017 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_SIZE_HPP
#define FOUNDATIONKIT_SIZE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <math.h>
#include <float.h>
#include "FoundationKit/Math/Vector2.hpp"
#include "FoundationKit/Math/Math.hpp"

NS_FK_BEGIN

struct Size
{
    typedef float value_type;
    value_type Width;
    value_type Height;

    operator Vector2() const
    {
        return Vector2(Width, Height);
    }

    Size() : Width(0), Height(0){}

    Size(value_type w, value_type h) : Width(w), Height(h){}

    explicit Size(const Vector2& point) : Width(point.X), Height(point.Y){}

    Size(const Size& other) : Width(other.Width), Height(other.Height){}

    inline Size& operator= (const Size& other)
    {
        SetSize(other.Width, other.Height);
        return *this;
    }

    inline Size& operator= (const Vector2& point)
    {
        SetSize(point.X, point.Y);
        return *this;
    }

    inline Size operator+(const Size& right) const
    {
        return Size(this->Width + right.Width, this->Height + right.Height);
    }

    inline Size operator-(const Size& right) const
    {
        return Size(this->Width - right.Width, this->Height - right.Height);
    }

    inline Size operator*(value_type a) const
    {
        return Size(this->Width * a, this->Height * a);
    }

    inline Size operator/(value_type a) const
    {
        return Size(this->Width / a, this->Height / a);
    }

    void SetSize(value_type width, value_type height)
    {
        this->Width = width;
        this->Height = height;
    }

    bool IsZero()
    {
        return (Width == 0 && Height == 0);
    }

    /// Returns a boolean indicating whether the given Vector2 is equal to this Vector2 instance.
    /// @param other The Vector2 to compare this instance to.
    /// @return True if the other Vector2 is equal to this instance; False otherwise.
    bool Equals(const Size& other)const
    {
        return (Math::Equal(this->Width, other.Width) && Math::Equal(this->Height, other.Height));
    }

    /// Returns a String representing this Vector2 instance.
    /// @return The string representation.
    std::string ToString()const
    {
        return StringUtils::Format("[Width:%f, Height:%f]", Width, Height);
    }

    /// Returns the hash code for this instance.
    /// @return The hash code.
    size_t GetHashCode()const
    {
        size_t hash = std::hash<value_type>{}(Width);
        stdx::hash_combine(hash, Height);
        return hash;
    }

    float* Ptr()
    {
        return reinterpret_cast<float*>(this);
    }

};

NS_FK_END

#endif // FOUNDATIONKIT_SIZE_HPP
