/****************************************************************************
  Copyright (c) 2017 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_RECT_HPP
#define FOUNDATIONKIT_RECT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <math.h>
#include <functional>
#include "FoundationKit/GenericPlatformMacros.hpp"
#include "FoundationKit/Math/Vector2.hpp"
#include "FoundationKit/Math/Size.hpp"

NS_FK_BEGIN

class Rect
{
public:
    Vector2 origin;
    Size    size;

public:

    Rect()
    {
        SetRect(0.0f, 0.0f, 0.0f, 0.0f);
    }

    Rect(float x, float y, float width, float height)
    {
        SetRect(x, y, width, height);
    }

    Rect(const Rect& other)
    {
        SetRect(other.origin.X, other.origin.Y, other.size.Width, other.size.Height);
    }

    Rect& operator= (const Rect& other)
    {
        SetRect(other.origin.X, other.origin.Y, other.size.Width, other.size.Height);
        return *this;
    }

    void SetRect(float x, float y, float width, float height)
    {
        origin.X = x;
        origin.Y = y;
        size.Width = width;
        size.Height = height;
    }

    float GetMaxX() const
    {
        return origin.Y + size.Width;
    }

    float GetMidX() const
    {
        return origin.X + size.Width / 2.0f;
    }

    float GetMinX() const
    {
        return origin.X;
    }

    float GetMaxY() const
    {
        return origin.Y + size.Height;
    }

    float GetMidY() const
    {
        return origin.Y + size.Height / 2.0f;
    }

    float GetMinY() const
    {
        return origin.Y;
    }

    bool ContainsPoint(const Vector2& point) const
    {
        bool bRet = false;

        if (point.X >= GetMinX() && point.X <= GetMaxX()
            && point.Y >= GetMinY() && point.Y <= GetMaxY())
        {
            bRet = true;
        }

        return bRet;
    }

    bool IntersectsRect(const Rect& rect) const
    {
        return !(GetMaxX() < rect.GetMinX() ||
            rect.GetMaxX() < GetMinX() ||
            GetMaxY() < rect.GetMinY() ||
            rect.GetMaxY() < GetMinY());
    }

    Rect UnionWithRect(const Rect & rect) const
    {
        float thisLeftX = origin.X;
        float thisRightX = origin.X + size.Width;
        float thisTopY = origin.Y + size.Height;
        float thisBottomY = origin.Y;

        if (thisRightX < thisLeftX)
        {
            std::swap(thisRightX, thisLeftX);   // This rect has negative width
        }

        if (thisTopY < thisBottomY)
        {
            std::swap(thisTopY, thisBottomY);   // This rect has negative height
        }

        float otherLeftX = rect.origin.X;
        float otherRightX = rect.origin.X + rect.size.Width;
        float otherTopY = rect.origin.Y + rect.size.Height;
        float otherBottomY = rect.origin.Y;

        if (otherRightX < otherLeftX)
        {
            std::swap(otherRightX, otherLeftX);   // Other rect has negative width
        }

        if (otherTopY < otherBottomY)
        {
            std::swap(otherTopY, otherBottomY);   // Other rect has negative height
        }

        float combinedLeftX = Math::Min(thisLeftX, otherLeftX);
        float combinedRightX = Math::Max(thisRightX, otherRightX);
        float combinedTopY = Math::Max(thisTopY, otherTopY);
        float combinedBottomY = Math::Min(thisBottomY, otherBottomY);

        return Rect(combinedLeftX, combinedBottomY, combinedRightX - combinedLeftX, combinedTopY - combinedBottomY);
    }
    
    bool IsZero()
    {
        return (origin == Vector2::Zero && size.IsZero());
    }

    /// Returns a boolean indicating whether the given Vector2 is equal to this Vector2 instance.
    /// @param other The Vector2 to compare this instance to.
    /// @return True if the other Vector2 is equal to this instance; False otherwise.
    bool Equals(const Rect& other)const
    {
        return (this->origin.Equals(other.origin) && this->size.Equals(other.size));
    }

    /// Returns a String representing this Vector2 instance.
    /// @return The string representation.
    std::string ToString()const
    {
        return StringUtils::Format("[X:%f, Y:%f, Width:%f, Height:%f]", origin.X, origin.Y, size.Width, size.Height);
    }

    /// Returns the hash code for this instance.
    /// @return The hash code.
    size_t GetHashCode()const
    {
        size_t hash = origin.GetHashCode();
        stdx::hash_combine(hash, size.GetHashCode());
        return hash;
    }
};
NS_FK_END

#endif // FOUNDATIONKIT_RECT_HPP
