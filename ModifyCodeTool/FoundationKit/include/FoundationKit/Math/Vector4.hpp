/****************************************************************************
  Copyright (c) 2018 libo All rights reserved.
  
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_VECTOR4_HPP
#define FOUNDATIONKIT_VECTOR4_HPP


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)
#include "FoundationKit/GenericPlatformMacros.hpp" // for NS_FK_BEGIN NS_FK_END
#include "FoundationKit/Foundation/StringUtils.hpp"
#include "FoundationKit/Base/functional.hpp"
#include "FoundationKit/Math/Vector2.hpp"
#include "FoundationKit/Math/Vector3.hpp"


NS_FK_BEGIN

struct Quaternion;
struct Matrix4x4;

/// A structure encapsulating four single precision value_typeing point values and provides hardware accelerated methods.
struct Vector4
{
    typedef float value_type;

    /// The X component of the vector.
    value_type X;
    /// The Y component of the vector.
    value_type Y;
    /// The Z component of the vector.
    value_type Z;
    /// The W component of the vector.
    value_type W;

    /// Returns the vector (0,0,0,0).
    static Vector4 Zero;
        /// Returns the vector (1,1,1,1).
    static Vector4 One;
        /// Returns the vector (1,0,0,0).
    static Vector4 UnitX;
        /// Returns the vector (0,1,0,0).
    static Vector4 UnitY;
        /// Returns the vector (0,0,1,0).
    static Vector4 UnitZ;
        /// Returns the vector (0,0,0,1).
    static Vector4 UnitW;

    Vector4()
        : Vector4(0.0f, 0.0f, 0.0f, 0.0f)
    {
    }

    /// Constructs a vector whose elements are all the single specified value.
    /// @param value The element to fill the vector with.
    Vector4(value_type value)
        : Vector4(value, value, value, value)
    {
    }

    /// Constructs a vector with the given individual elements.

    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @param w W component.
    Vector4(value_type x, value_type y, value_type z, value_type w)
    {
        W = w;
        X = x;
        Y = y;
        Z = z;
    }

    /// Constructs a Vector4 from the given Vector2 and a Z and W component.
    /// @param value The vector to use as the X and Y components.
    /// @param z Z component.
    /// @param w W component.
    Vector4(const Vector2& value, value_type z, value_type w)
    {
        X = value.X;
        Y = value.Y;
        Z = z;
        W = w;
    }


    /// Constructs a Vector4 from the given Vector3 and a W component.
    /// @param value The vector to use as the X, Y, and Z components.
    /// @param w The W component.
    Vector4(const Vector3& value, value_type w)
    {
        X = value.X;
        Y = value.Y;
        Z = value.Z;
        W = w;
    }

    /// Returns a boolean indicating whether the given Vector4 is equal to this Vector4 instance.
    /// @param other The Vector4 to compare this instance to.
    /// @return True if the other Vector4 is equal to this instance; False otherwise.
    bool Equals(const Vector4& other)const
    {
        return this->X == other.X
            && this->Y == other.Y
            && this->Z == other.Z
            && this->W == other.W;
    }

    /// Returns a String representing this Vector4 instance.
    /// @return The string representation.
    std::string ToString()const
    {
        return StringUtils::Format("[X:%f, Y:%f, Z:%f, W:%f]", X, Y, Z, W);
    }

    /// Returns the hash code for this instance.
    /// @return The hash code.
    size_t GetHashCode()
    {
        size_t hash = std::hash<value_type>{}(X);
        stdx::hash_combine(hash, Y);
        stdx::hash_combine(hash, Z);
        stdx::hash_combine(hash, W);
        return hash;
    }

    /// Returns the length of the vector. This operation is cheaper than Length().
    /// @return The vector's length.
    value_type Length()const
    {
        value_type ls = Vector4::Dot(*this, *this);
        return (value_type)Math::Sqrt(ls);
    }

    /// Returns the length of the vector squared.
    /// @return The vector's length squared.
    value_type LengthSquared()
    {
        return Vector4::Dot(*this, *this);
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
    static value_type Distance(const Vector4& value1, const Vector4& value2)
    {
        Vector4 difference = value1 - value2;
        value_type ls = Vector4::Dot(difference, difference);
        return (value_type)Math::Sqrt(ls);
    }

    /// Returns the Euclidean distance squared between the two given points.
    /// @param value1 The first point.
    /// @param value2 The second point.
    /// @return The distance squared.
    static value_type DistanceSquared(const Vector4& value1, const Vector4& value2)
    {
        Vector4 difference = value1 - value2;
        return Vector4::Dot(difference, difference);
    }

    /// Returns a vector with the same direction as the given vector, but with a length of 1.
    /// @param vector The vector to normalize.
    /// @return The normalized vector.
    static Vector4 Normalize(const Vector4& vector)
    {
        value_type length = vector.Length();
        return vector / length;
    }

    /// Restricts a vector between a min and max value.
    /// @param value1 The source vector.
    /// @param min The minimum value.
    /// @param max The maximum value.
    /// @return The restricted vector.
    static Vector4 Clamp(const Vector4& value1, const Vector4& min, const Vector4& max)
    {
        // This compare order is very important!!!
        // We must follow HLSL behavior in the case user specified min value is bigger than max value.

        value_type x = value1.X;
        x = (x > max.X) ? max.X : x;
        x = (x < min.X) ? min.X : x;

        value_type y = value1.Y;
        y = (y > max.Y) ? max.Y : y;
        y = (y < min.Y) ? min.Y : y;

        value_type z = value1.Z;
        z = (z > max.Z) ? max.Z : z;
        z = (z < min.Z) ? min.Z : z;

        value_type w = value1.W;
        w = (w > max.W) ? max.W : w;
        w = (w < min.W) ? min.W : w;

        return Vector4(x, y, z, w);
    }

    /// Linearly interpolates between two vectors based on the given weighting.
    /// @param value1 The first source vector.
    /// @param value2 The second source vector.
    /// @param amount Value between 0 and 1 indicating the weight of the second source vector.
    /// @return The interpolated vector.
    static Vector4 Lerp(const Vector4& value1, const Vector4& value2, value_type amount)
    {
        return Vector4(
            value1.X + (value2.X - value1.X) * amount,
            value1.Y + (value2.Y - value1.Y) * amount,
            value1.Z + (value2.Z - value1.Z) * amount,
            value1.W + (value2.W - value1.W) * amount);
    }

    /// Transforms a vector by the given matrix.
    /// @param position The source vector.
    /// @param matrix The transformation matrix.
    /// @return The transformed vector.
    static Vector4 Transform(const Vector2& position, const Matrix4x4& matrix);

    /// Transforms a vector by the given matrix.
    /// @param position The source vector.
    /// @param matrix The transformation matrix.
    /// @return The transformed vector.
    static Vector4 Transform(const Vector3& position, const Matrix4x4& matrix);

    /// Transforms a vector by the given matrix.
    /// @param vector The source vector.
    /// @param matrix The transformation matrix.
    /// @return The transformed vector.
    static Vector4 Transform(const Vector4& vector, const Matrix4x4& matrix);

    /// Transforms a vector by the given Quaternion rotation value.
    /// @param value The source vector to be rotated.
    /// @param rotation The rotation to apply.
    /// @return The transformed vector.
    static Vector4 Transform(const Vector2& value, const Quaternion& rotation);

    /// Transforms a vector by the given Quaternion rotation value.
    /// @param value The source vector to be rotated.
    /// @param rotation The rotation to apply.
    /// @return The transformed vector.
    static Vector4 Transform(const Vector3& value, const Quaternion& rotation);

    /// Transforms a vector by the given Quaternion rotation value.
    /// @param value The source vector to be rotated.
    /// @param rotation The rotation to apply.
    /// @return The transformed vector.
    static Vector4 Transform(const Vector4& value, const Quaternion& rotation);

    /// Adds two vectors together.
    /// @param left The first source vector.
    /// @param right The second source vector.
    /// @return The summed vector.
    static Vector4 Add(const Vector4& left, const Vector4& right)
    {
        return left + right;
    }

    /// Subtracts the second vector from the first.
    /// @param left The first source vector.
    /// @param right The second source vector.
    /// @return The difference vector.
    static Vector4 Subtract(const Vector4& left, const Vector4& right)
    {
        return left - right;
    }

    /// Multiplies two vectors together.
    /// @param left The first source vector.
    /// @param right The second source vector.
    /// @return The product vector.
    static Vector4 Multiply(const Vector4& left, const Vector4& right)
    {
        return left * right;
    }

    /// Multiplies a vector by the given scalar.
    /// @param left The source vector.
    /// @param right The scalar value.
    /// @return The scaled vector.
    static Vector4 Multiply(const Vector4& left, value_type right)
    {
        return left * Vector4(right, right, right, right);
    }

    /// Multiplies a vector by the given scalar.
    /// @param left The scalar value.
    /// @param right The source vector.
    /// @return The scaled vector.
    static Vector4 Multiply(value_type left, const Vector4& right)
    {
        return Vector4(left, left, left, left) * right;
    }

    /// Divides the first vector by the second.
    /// @param left The first source vector.
    /// @param right The second source vector.
    /// @return The vector resulting from the division.
    static Vector4 Divide(const Vector4& left, const Vector4& right)
    {
        return left / right;
    }

    /// Divides the vector by the given scalar.

    /// @param left The source vector.
    /// @param divisor The scalar value.
    /// @return The result of the division.
    static Vector4 Divide(const Vector4& left, value_type divisor)
    {
        return left / divisor;
    }

    /// Negates a given vector.
    /// @param value The source vector.
    /// @return The negated vector.
    static Vector4 Negate(const Vector4& value)
    {
        return -value;
    }

    /// Returns the dot product of two vectors.
    /// @param vector1 The first vector.
    /// @param vector2 The second vector.
    /// @return The dot product.
    static value_type Dot(const Vector4& vector1, const Vector4& vector2)
    {
        return vector1.X * vector2.X +
            vector1.Y * vector2.Y +
            vector1.Z * vector2.Z +
            vector1.W * vector2.W;
    }

    /// Returns a vector whose elements are the minimum of each of the pairs of elements in the two source vectors.
    /// @param value1 The first source vector.
    /// @param value2 The second source vector.
    /// @return The minimized vector.
    static Vector4 Min(const Vector4& value1, const Vector4& value2)
    {
        return Vector4(
            (value1.X < value2.X) ? value1.X : value2.X,
            (value1.Y < value2.Y) ? value1.Y : value2.Y,
            (value1.Z < value2.Z) ? value1.Z : value2.Z,
            (value1.W < value2.W) ? value1.W : value2.W);
    }

    /// Returns a vector whose elements are the maximum of each of the pairs of elements in the two source vectors.
    /// @param value1 The first source vector.
    /// @param value2 The second source vector.
    /// @return The maximized vector.
    static Vector4 Max(const Vector4& value1, const Vector4& value2)
    {
        return Vector4(
            (value1.X > value2.X) ? value1.X : value2.X,
            (value1.Y > value2.Y) ? value1.Y : value2.Y,
            (value1.Z > value2.Z) ? value1.Z : value2.Z,
            (value1.W > value2.W) ? value1.W : value2.W);
    }

    /// Returns a vector whose elements are the absolute values of each of the source vector's elements.
    /// @param value The source vector.
    /// @return The absolute value vector.
    static Vector4 Abs(const Vector4& value)
    {
        return Vector4(Math::Abs(value.X), Math::Abs(value.Y), Math::Abs(value.Z), Math::Abs(value.W));
    }

    /// Returns a vector whose elements are the square root of each of the source vector's elements.
    /// @param value The source vector.
    /// @return The square root vector.
    static Vector4 SquareRoot(const Vector4& value)
    {
        return Vector4((value_type)Math::Sqrt(value.X), (value_type)Math::Sqrt(value.Y), (value_type)Math::Sqrt(value.Z), (value_type)Math::Sqrt(value.W));
    }

public: /// operator override

    /// Adds two vectors together.
    /// @param right The source vector.
    /// @return The summed vector.
    inline Vector4 operator +(const Vector4& right)const
    {
        return Vector4(this->X + right.X, this->Y + right.Y, this->Z + right.Z, this->W + right.W);
    }

    /// Subtracts the second vector from the first.
    /// @param right The source vector.
    /// @return The difference vector.
    inline Vector4 operator -(const Vector4& right)const
    {
        return Vector4(this->X - right.X, this->Y - right.Y, this->Z - right.Z, this->W - right.W);
    }

    /// Multiplies two vectors together.
    /// @param right The source vector.
    /// @return The product vector.
    inline Vector4 operator *(const Vector4& right)const
    {
        return Vector4(this->X * right.X, this->Y * right.Y, this->Z * right.Z, this->W * right.W);
    }

    /// Multiplies a vector by the given scalar.

    /// @param right The scalar value.
    /// @return The scaled vector.
    inline Vector4 operator *(value_type right)const
    {
        return (*this) * Vector4(right);
    }

    /// Divides the first vector by the second.
    /// @param right The source vector.
    /// @return The vector resulting from the division.
    inline Vector4 operator /(const Vector4& right)const
    {
        return Vector4(this->X / right.X, this->Y / right.Y, this->Z / right.Z, this->W / right.W);
    }

    /// Divides the vector by the given scalar.
    /// @param right The scalar value.
    /// @return The result of the division.
    inline Vector4 operator /( value_type right)const
    {
        Vector4::value_type invDiv = 1.0f / right;

        return Vector4(
            this->X * invDiv,
            this->Y * invDiv,
            this->Z * invDiv,
            this->W * invDiv);
    }

    /// Negates a given vector.
    /// @return The negated vector.
    inline Vector4 operator -()const
    {
        return Vector4::Zero - (*this);
    }

    /// Returns a boolean indicating whether the two given vectors are equal.
    /// @param right The second vector to compare.
    /// @return True if the vectors are equal; False otherwise.
    inline bool operator ==(const Vector4& right)
    {
        return this->Equals(right);
    }

    /// Returns a boolean indicating whether the two given vectors are not equal.
    /// @param right The second vector to compare.
    /// @return True if the vectors are not equal; False if they are equal.
    inline bool operator !=( const Vector4& right)
    {
        return !((*this) == right);
    }
};


///// Adds two vectors together.
///// @param left The first source vector.
///// @param right The second source vector.
///// @return The summed vector.
//static Vector4 operator +(const Vector4& left, const Vector4& right)
//{
//    return Vector4(left.X + right.X, left.Y + right.Y, left.Z + right.Z, left.W + right.W);
//}
//
///// Subtracts the second vector from the first.
///// @param left The first source vector.
///// @param right The second source vector.
///// @return The difference vector.
//static Vector4 operator -(const Vector4& left, const Vector4& right)
//{
//    return Vector4(left.X - right.X, left.Y - right.Y, left.Z - right.Z, left.W - right.W);
//}
//
///// Multiplies two vectors together.
///// @param left The first source vector.
///// @param right The second source vector.
///// @return The product vector.
//static Vector4 operator *(const Vector4& left, const Vector4& right)
//{
//    return Vector4(left.X * right.X, left.Y * right.Y, left.Z * right.Z, left.W * right.W);
//}
//
///// Multiplies a vector by the given scalar.
///// @param left The source vector.
///// @param right The scalar value.
///// @return The scaled vector.
//static Vector4 operator *(const Vector4& left, Vector4::value_type right)
//{
//    return left * Vector4(right);
//}
//
///// Multiplies a vector by the given scalar.
///// @param left The scalar value.
///// @param right The source vector.
///// @return The scaled vector.
//static Vector4 operator *(Vector4::value_type left, const Vector4& right)
//{
//    return Vector4(left) * right;
//}
//
///// Divides the first vector by the second.
///// @param left The first source vector.
///// @param right The second source vector.
///// @return The vector resulting from the division.
//static Vector4 operator /(const Vector4& left, const Vector4& right)
//{
//    return Vector4(left.X / right.X, left.Y / right.Y, left.Z / right.Z, left.W / right.W);
//}
//
///// Divides the vector by the given scalar.
///// @param value1 The source vector.
///// @param value2 The scalar value.
///// @return The result of the division.
//static Vector4 operator /(const Vector4& value1, Vector4::value_type value2)
//{
//    Vector4::value_type invDiv = 1.0f / value2;
//
//    return Vector4(
//        value1.X * invDiv,
//        value1.Y * invDiv,
//        value1.Z * invDiv,
//        value1.W * invDiv);
//}
//
///// Negates a given vector.
///// @param value The source vector.
///// @return The negated vector.
//static Vector4 operator -(const Vector4& value)
//{
//    return Vector4::Zero - value;
//}
//
///// Returns a boolean indicating whether the two given vectors are equal.
///// @param left The first vector to compare.
///// @param right The second vector to compare.
///// @return True if the vectors are equal; False otherwise.
//static bool operator ==(const Vector4& left, const Vector4& right)
//{
//    return left.Equals(right);
//}
//
///// Returns a boolean indicating whether the two given vectors are not equal.
///// @param left The first vector to compare.
///// @param right The second vector to compare.
///// @return True if the vectors are not equal; False if they are equal.
//static bool operator !=(const Vector4& left, const Vector4& right)
//{
//    return !(left == right);
//}

NS_FK_END

#endif // END OF FOUNDATIONKIT_VECTOR4_HPP
