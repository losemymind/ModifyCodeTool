/****************************************************************************
  Copyright (c) 2018 libo All rights reserved.
  
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_VECTOR2_HPP
#define FOUNDATIONKIT_VECTOR2_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "FoundationKit/GenericPlatformMacros.hpp"
#include "FoundationKit/Foundation/StringUtils.hpp"
#include "FoundationKit/Base/functional.hpp"
#include "FoundationKit/Math/Math.hpp"

NS_FK_BEGIN
struct Matrix4x4;
struct Matrix3x2;
struct Quaternion;
/// A structure encapsulating two single precision value_typeing point values and provides hardware accelerated methods.
struct Vector2
{
    typedef float value_type;
    /// The X component of the vector.
    value_type X;
    /// The Y component of the vector.
    value_type Y;

    /// Returns the vector (0,0).
    static Vector2 Zero;
    /// Returns the vector (1,1).
    static Vector2 One;
        /// Returns the vector (1,0).
    static Vector2 UnitX;
        /// Returns the vector (0,1).
    static Vector2 UnitY;

    /// Default constructor.
    Vector2() :Vector2(0.0f) {}
    /// Constructs a vector whose elements are all the single specified value.
    /// @param value The element to fill the vector with.
    Vector2(value_type value) : Vector2(value, value) { }

    /// Constructs a vector with the given individual elements.
    /// @param x The X component.
    /// @param y The Y component.
    Vector2(value_type x, value_type y): X(x), Y(y){ }

    /// Returns a boolean indicating whether the given Vector2 is equal to this Vector2 instance.
    /// @param other The Vector2 to compare this instance to.
    /// @return True if the other Vector2 is equal to this instance; False otherwise.
    bool Equals(const Vector2& other)const
    {
        return this->X == other.X && this->Y == other.Y;
    }

    /// Returns a String representing this Vector2 instance.
    /// @return The string representation.
    std::string ToString()const
    {
        return StringUtils::Format("[X:%f, Y:%f]", X, Y);
    }

    /// Returns the hash code for this instance.
    /// @return The hash code.
    size_t GetHashCode()const
    {
        size_t hash = std::hash<value_type>{}(X);
        stdx::hash_combine(hash, Y);
        return hash;
    }

    /// Returns the length of the vector.
    /// @return The vector's length.
    value_type Length()const
    {
        value_type ls = Vector2::Dot(*this, *this);
        return (value_type)Math::Sqrt(ls);
    }

    /// Returns the length of the vector squared. This operation is cheaper than Length().
    /// @return The vector's length squared.
    value_type LengthSquared()const
    {
        return Vector2::Dot(*this, *this);
    }

    value_type* Ptr()
    {
        return reinterpret_cast<value_type*>(this);
    }

public: /// static functions

    /// Returns the Euclidean distance between the two given points.
    /// @param value1 The first point.
    /// @param value2 The second point.
    /// @return The distance.
    static value_type Distance(const Vector2& value1, const Vector2& value2)
    {
        Vector2 difference = value1 - value2;
        value_type ls = Vector2::Dot(difference, difference);
        return (value_type)Math::Sqrt(ls);
    }

    /// Returns the Euclidean distance squared between the two given points.
    /// @param value1 The first point.
    /// @param value2 The second point.
    /// @return The distance squared.
    static value_type DistanceSquared(const Vector2& value1, const Vector2& value2)
    {
        Vector2 difference = value1 - value2;
        return Vector2::Dot(difference, difference);
    }

    /// Returns a vector with the same direction as the given vector, but with a length of 1.
    /// @param value The vector to normalize.
    /// @return The normalized vector.
    static Vector2 Normalize(const Vector2& value)
    {
        value_type length = value.Length();
        return value / length;
    }


    /// Returns the reflection of a vector off a surface that has the specified normal.
    /// @param vector The source vector.
    /// @param normal The normal of the surface being reflected off.
    /// @return The reflected vector.
    static Vector2 Reflect(const Vector2& vector, const Vector2& normal)
    {
        value_type dot = Vector2::Dot(vector, normal);
        return vector - (normal*(2 * dot));
    }

    /// Restricts a vector between a min and max value.
    /// @param value1 The source vector.
    /// @param min The minimum value.
    /// @param max The maximum value.
    static Vector2 Clamp(const Vector2& value1, const Vector2& min, const Vector2& max)
    {
        // This compare order is very important!!!
        // We must follow HLSL behavior in the case user specified min value is bigger than max value.
        value_type x = value1.X;
        x = (x > max.X) ? max.X : x;
        x = (x < min.X) ? min.X : x;

        value_type y = value1.Y;
        y = (y > max.Y) ? max.Y : y;
        y = (y < min.Y) ? min.Y : y;

        return Vector2(x, y);
    }

    /// Linearly interpolates between two vectors based on the given weighting.
    /// @param value1 The first source vector.
    /// @param value2 The second source vector.
    /// @param amount Value between 0 and 1 indicating the weight of the second source vector.
    /// @return The interpolated vector.
    static Vector2 Lerp(const Vector2& value1, const Vector2& value2, value_type amount)
    {
        return Vector2(
            value1.X + (value2.X - value1.X) * amount,
            value1.Y + (value2.Y - value1.Y) * amount);
    }

    /// Transforms a vector by the given matrix.
    /// @param position The source vector.
    /// @param matrix The transformation matrix.
    /// @return The transformed vector.
    static Vector2 Transform(const Vector2& position, const Matrix3x2& matrix);

    /// Transforms a vector by the given matrix.
    /// @param position The source vector.
    /// @param matrix The transformation matrix.
    /// @return The transformed vector.
    static Vector2 Transform(const Vector2& position, const Matrix4x4& matrix);

    /// Transforms a vector normal by the given matrix.
    /// @param normal The source vector.
    /// @param matrix The transformation matrix.
    /// @return The transformed vector.
    static Vector2 TransformNormal(const Vector2& normal, const Matrix3x2& matrix);

    /// Transforms a vector normal by the given matrix.
    /// @param normal The source vector.
    /// @param matrix The transformation matrix.
    /// @return The transformed vector.
    static Vector2 TransformNormal(const Vector2& normal, const Matrix4x4& matrix);

    /// Transforms a vector by the given Quaternion rotation value.
    /// @param value The source vector to be rotated.
    /// @param rotation The rotation to apply.
    /// @return The transformed vector.
    static Vector2 Transform(const Vector2& value, const Quaternion& rotation);

    /// Adds two vectors together.
    /// @param left The first source vector.
    /// @param right The second source vector.
    /// @return The summed vector.
    static Vector2 Add(const Vector2& left, const Vector2& right)
    {
        return left + right;
    }

    /// Subtracts the second vector from the first.
    /// @param left The first source vector.
    /// @param right The second source vector.
    /// @return The difference vector.
    static Vector2 Subtract(const Vector2& left, const Vector2& right)
    {
        return left - right;
    }

    /// Multiplies two vectors together.
    /// @param left The first source vector.
    /// @param right The second source vector.
    /// @return The product vector.
    static Vector2 Multiply(const Vector2& left, const Vector2& right)
    {
        return left * right;
    }

    /// Multiplies a vector by the given scalar.
    /// @param left The source vector.
    /// @param right The scalar value.
    /// @return The scaled vector.
    static Vector2 Multiply(const Vector2& left, value_type right)
    {
        return left * right;
    }

    /// Multiplies a vector by the given scalar.
    /// @param left The scalar value.
    /// @param right The source vector.
    /// @return The scaled vector.
    static Vector2 Multiply(value_type left, const Vector2& right)
    {
        return right * left;
    }

    /// Divides the first vector by the second.
    /// @param left The first source vector.
    /// @param right The second source vector.
    /// @return The vector resulting from the division.
    static Vector2 Divide(const Vector2& left, const Vector2& right)
    {
        return left / right;
    }

    /// Divides the vector by the given scalar.
    /// @param left The source vector.
    /// @param divisor The scalar value.
    /// @return The result of the division.
    static Vector2 Divide(const Vector2& left, value_type divisor)
    {
        return left / divisor;
    }

    /// Negates a given vector.
    /// @param value The source vector.
    /// @return The negated vector.
    static Vector2 Negate(const Vector2& value)
    {
        return -value;
    }

    /// Returns the dot product of two vectors.
    /// @param value1 The first vector.
    /// @param value2 The second vector.
    /// @return The dot product.
    static value_type Dot(const Vector2& value1, const Vector2& value2)
    {
        return value1.X * value2.X +
            value1.Y * value2.Y;
    }

    /// Returns a vector whose elements are the minimum of each of the pairs of elements in the two source vectors.
    /// @param value1 The first source vector.
    /// @param value2 The second source vector.
    /// @return The minimized vector.
    static Vector2 Min(const Vector2& value1, const Vector2& value2)
    {
        return Vector2(
            (value1.X < value2.X) ? value1.X : value2.X,
            (value1.Y < value2.Y) ? value1.Y : value2.Y);
    }

    /// Returns a vector whose elements are the maximum of each of the pairs of elements in the two source vectors
    /// @param value1 The first source vector
    /// @param value2 The second source vector
    /// @return The maximized vector
    static Vector2 Max(const Vector2& value1, const Vector2& value2)
    {
        return Vector2(
            (value1.X > value2.X) ? value1.X : value2.X,
            (value1.Y > value2.Y) ? value1.Y : value2.Y);
    }

    /// Returns a vector whose elements are the absolute values of each of the source vector's elements.
    /// @param value The source vector.
    /// @return The absolute value vector.
    static Vector2 Abs(const Vector2& value)
    {
        return Vector2(Math::Abs(value.X), Math::Abs(value.Y));
    }

    /// Returns a vector whose elements are the square root of each of the source vector's elements.
    /// @param value The source vector.
    /// @return The square root vector.
    static Vector2 SquareRoot(const Vector2& value)
    {
        return Vector2((value_type)Math::Sqrt(value.X), (value_type)Math::Sqrt(value.Y));
    }

public: /// operator override

    /// Adds two vectors together.
    /// @param right The source vector.
    /// @return The summed vector.
    inline Vector2 operator +(const Vector2& right)const
    {
        return Vector2(this->X + right.X, this->Y + right.Y);
    }

    /// Subtracts the second vector from the first.
    /// @param right The source vector.
    /// @return The difference vector.
    inline Vector2 operator -(const Vector2& right)const
    {
        return Vector2(this->X - right.X, this->Y - right.Y);
    }

    /// Multiplies two vectors together.
    /// @param right The source vector.
    /// @return The product vector.
    inline Vector2 operator *(const Vector2& right)const
    {
        return Vector2(this->X * right.X, this->Y * right.Y);
    }

    /// Multiplies a vector by the given scalar.
    /// @param right The scalar value.
    /// @return The scaled vector.
    inline Vector2 operator *(value_type right)const
    {
        return (*this) * Vector2(right, right);
    }

    /// Divides the first vector by the second.
    /// @param right The source vector.
    /// @return The vector resulting from the division.
    inline Vector2 operator /(const Vector2& right)const
    {
        return Vector2(this->X / right.X, this->Y / right.Y);
    }

    /// Divides the vector by the given scalar.
    /// @param right The scalar value.
    /// @return The result of the division.
    inline Vector2 operator /(Vector2::value_type right)const
    {
        Vector2::value_type invDiv = 1.0f / right;
        return Vector2( this->X * invDiv, this->Y * invDiv);
    }

    /// Negates a given vector.
    /// @return The negated vector.
    inline Vector2 operator -()const
    {
        return Vector2::Zero - (*this);
    }

    /// Returns a boolean indicating whether the two given vectors are equal.
    /// @param right The vector to compare.
    /// @return True if the vectors are equal; False otherwise.
    inline bool operator ==(const Vector2& right)const
    {
        return this->Equals(right);
    }

    /// Returns a boolean indicating whether the two given vectors are not equal.
    /// @param right The second to compare.
    /// @return True if the vectors are not equal; False if they are equal.
    inline bool operator !=(const Vector2& right) const
    {
        return !((*this) == right);
    }
};
//
///// Adds two vectors together.
///// @param left The first source vector.
///// @param right The second source vector.
///// @return The summed vector.
//static inline Vector2 operator +(const Vector2& left, const Vector2& right)
//{
//    return Vector2(left.X + right.X, left.Y + right.Y);
//}
//
///// Subtracts the second vector from the first.
///// @param left The first source vector.
///// @param right The second source vector.
///// @return The difference vector.
//static inline Vector2 operator -(const Vector2& left, const Vector2& right)
//{
//    return Vector2(left.X - right.X, left.Y - right.Y);
//}
//
///// Multiplies two vectors together.
///// @param left The first source vector.
///// @param right The second source vector.
///// @return The product vector.
//static inline Vector2 operator *(const Vector2& left, const Vector2& right)
//{
//    return Vector2(left.X * right.X, left.Y * right.Y);
//}
//
///// Multiplies a vector by the given scalar.
///// @param left The scalar value.
///// @param right The source vector.
///// @return The scaled vector.
//static inline Vector2 operator *(Vector2::value_type left, const Vector2& right)
//{
//    return Vector2(left, left) * right;
//}
//
///// Multiplies a vector by the given scalar.
///// @param left The source vector.
///// @param right The scalar value.
///// @return The scaled vector.
//static inline Vector2 operator *(const Vector2& left, Vector2::value_type right)
//{
//    return left * Vector2(right, right);
//}
//
///// Divides the first vector by the second.
///// @param left The first source vector.
///// @param right The second source vector.
///// @return The vector resulting from the division.
//static inline Vector2 operator /(const Vector2& left, const Vector2& right)
//{
//    return Vector2(left.X / right.X, left.Y / right.Y);
//}
//
///// Divides the vector by the given scalar.
///// @param value1 The source vector.
///// @param value2 The scalar value.
///// @return The result of the division.
//static inline Vector2 operator /(const Vector2& value1, Vector2::value_type value2)
//{
//    Vector2::value_type invDiv = 1.0f / value2;
//    return Vector2(
//        value1.X * invDiv,
//        value1.Y * invDiv);
//}
//
///// Negates a given vector.
///// @param value The source vector.
///// @return The negated vector.
//static inline Vector2 operator -(const Vector2& value)
//{
//    return Vector2::Zero - value;
//}
//
///// Returns a boolean indicating whether the two given vectors are equal.
///// @param left The first vector to compare.
///// @param right The second vector to compare.
///// @return True if the vectors are equal; False otherwise.
//static inline bool operator ==(const Vector2& left, const Vector2& right)
//{
//    return left.Equals(right);
//}
//
///// Returns a boolean indicating whether the two given vectors are not equal.
///// @param left The first vector to compare.
///// @param right The second vector to compare.
///// @return True if the vectors are not equal; False if they are equal.
//static inline bool operator !=(const Vector2& left, const Vector2& right)
//{
//    return !(left == right);
//}

NS_FK_END

#endif // END OF FOUNDATIONKIT_VECTOR2_HPP
