/****************************************************************************
Copyright (c) 2018 libo All rights reserved.

losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_VECTOR3_HPP
#define FOUNDATIONKIT_VECTOR3_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cstddef>
#include <functional>
#include "FoundationKit/GenericPlatformMacros.hpp" // for NS_FK_BEGIN NS_FK_END
#include "FoundationKit/Base/functional.hpp"
#include "FoundationKit/Math/Math.hpp"
#include "FoundationKit/Foundation/StringUtils.hpp"

NS_FK_BEGIN
struct Matrix4x4;
struct Quaternion;
/// A structure encapsulating three single precision value_typeing point values and provides hardware accelerated methods.
struct Vector3
{  
    typedef float value_type;
    value_type X;///The x-coordinate.
    value_type Y;/// The y-coordinate.
    value_type Z;/// The z-coordinate.

	static  Vector3 ZERO;     /// Vector3(0f, 0f, 0f);
	static  Vector3 ONE;      /// Vector3(1f, 1f, 1f);
	static  Vector3 UNITX;    /// Vector3(1f, 0f, 0f);
	static  Vector3 UNITY;    /// Vector3(0f, 1f, 0f);
	static  Vector3 UNITZ;    /// Vector3(0f, 0f, 1f);
	static  Vector3 UP;       /// Vector3(0f, 1f, 0f);
	static  Vector3 DOWN;     /// Vector3(0f, -1f, 0f);
	static  Vector3 RIGHT;    /// Vector3(1f, 0f, 0f);
	static  Vector3 LEFT;     /// Vector3(-1f, 0f, 0f);
	static  Vector3 FORWARD;  /// Vector3(0f, 0f, -1f);
	static  Vector3 BACKWARD; /// Vector3(0f, 0f, 1f);


	Vector3() :Vector3(0.0f, 0.0f, 0.0f) {}

	/// Constructs a vector whose elements are all the single specified value.
	/// @param value The element to fill the vector with.
	Vector3(value_type value) : Vector3(value, value, value) { }

	/// Constructs a vector with the given individual elements.
	/// @param x The X component.
	/// @param y The Y component.
	/// @param z The Z component.
	Vector3(value_type x, value_type y, value_type z = 0.0f)
		: X(x)
		, Y(y)
		, Z(z)
	{

	}

	/// Constructs a new vector from the values in the specified array.
    /// @param values An array containing the elements of the vector in the order x, y, z.
	explicit Vector3(const value_type* values)
	{
		X = values[0];
		Y = values[1];
		Z = values[2];
	}

    /// Returns a boolean indicating whether the given Vector3 is equal to this Vector3 instance.
    /// @param other The Vector3 to compare this instance to.
    /// @return True if the other Vector3 is equal to this instance; False otherwise.
    bool Equals(const Vector3& other)const
    {
        return (X == other.X &&Y == other.Y &&Z == other.Z);
    }

    /// Returns a String representing this Vector3 instance.
    /// @return The string representation.
    std::string ToString()const
    {
        return StringUtils::Format("[X:%f, Y:%f, Z:%f]", X, Y, Z);
    }

	/// Returns the hash code for this instance.
	///@return The hash code.
	size_t GetHashCode()const
	{
		size_t hash = std::hash<value_type>{}(X);
		stdx::hash_combine(hash, Y);
		stdx::hash_combine(hash, Z);
		return hash;
	}

	/// Returns the length of the vector.
	/// @return The vector's length.
	inline value_type Length()const
	{

		value_type ls = X * X + Y * Y + Z * Z;
		return Math::Sqrt(ls);
	}

	/// Returns the length of the vector squared. This operation is cheaper than Length().
	/// @return The vector's length squared.
	inline value_type LengthSquared()const
	{
		return X * X + Y * Y + Z * Z;
	}

    value_type* Ptr()
    {
        return reinterpret_cast<value_type*>(this);
    }

	inline Vector3& operator = (const Vector3& other)
	{
		this->X = other.X;
		this->Y = other.Y;
		this->Z = other.Z;
		return (*this);
	}

public: /// static functions

	/// Transforms a vector by the given matrix.
	/// @param position The source vector.
	/// @param matrix The transformation matrix.
	/// @return The transformed vector.
	static Vector3 Transform(const Vector3& position, const Matrix4x4& matrix);

	/// Transforms a vector normal by the given matrix.
	/// @param normal The source vector.
	/// @param matrix The transformation matrix.
	/// @return The transformed vector.
	static Vector3 TransformNormal(const Vector3& normal, const Matrix4x4& matrix);

    /// Transforms a vector by the given Quaternion rotation value.
    /// @param value The source vector to be rotated.
    /// @param rotation The rotation to apply.
    /// @return The transformed vector.
    static Vector3 Transform(const Vector3& value, const Quaternion& rotation);

    /// Returns the Euclidean distance between the two given points.
    /// @param value1 The first point.
    /// @param value2 The second point.
    /// @return The distance.
    static inline float Distance(Vector3 value1, Vector3 value2)
    {
        Vector3 difference = value1 - value2;
        float ls = Vector3::Dot(difference, difference);
        return (float)Math::Sqrt(ls);
    }

    /// Returns the Euclidean distance squared between the two given points.
    /// @param value1 The first point.
    /// @param value2 The second point.
    /// @return The distance squared.
    static inline float DistanceSquared(Vector3 value1, Vector3 value2)
    {
        Vector3 difference = value1 - value2;
        return Vector3::Dot(difference, difference);
    }


	/// Returns a vector with the same direction as the given vector, but with a length of 1.
	/// @param value The vector to normalize.
	/// @return The normalized vector.
	static inline Vector3 Normalize(const Vector3& value)
	{
		value_type ls = value.X * value.X + value.Y * value.Y + value.Z * value.Z;
		value_type length = (value_type)Math::Sqrt(ls);
		return Vector3(value.X / length, value.Y / length, value.Z / length);
	}

	/// Computes the cross product of two vectors.
	/// @param vector1 The first vector.
	/// @param vector2 The second vector.
	/// @return The cross product.
	static inline Vector3 Cross(const Vector3& vector1, const Vector3& vector2)
	{
		return Vector3(
			vector1.Y * vector2.Z - vector1.Z * vector2.Y,
			vector1.Z * vector2.X - vector1.X * vector2.Z,
			vector1.X * vector2.Y - vector1.Y * vector2.X);
	}

	/// Returns the reflection of a vector off a surface that has the specified normal.
	/// @param vector The source vector.
	/// @param normal The normal of the surface being reflected off.
	/// @return The reflected vector.
	static inline Vector3 Reflect(const Vector3& vector, const Vector3& normal)
	{
		value_type dot = vector.X * normal.X + vector.Y * normal.Y + vector.Z * normal.Z;
		value_type tempX = normal.X * dot * 2.0f;
		value_type tempY = normal.Y * dot * 2.0f;
		value_type tempZ = normal.Z * dot * 2.0f;
		return Vector3(vector.X - tempX, vector.Y - tempY, vector.Z - tempZ);
	}

	/// Restricts a vector between a min and max value.
	/// @param value1 The source vector.
	/// @param min    The minimum value.
	/// @param max    The maximum value.
	/// @return The restricted vector.
	static inline Vector3 Clamp(const Vector3& value1, const Vector3& min, const Vector3& max)
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

		return Vector3(x, y, z);
	}

	/// Linearly interpolates between two vectors based on the given weighting.
	/// @param value1 The first source vector.
	/// @param value2 The second source vector.
	/// @param amount Value between 0 and 1 indicating the weight of the second source vector.
	/// @return The interpolated vector.
	static inline Vector3 Lerp(const Vector3& value1, const Vector3& value2, value_type amount)
	{
		return Vector3(
			value1.X + (value2.X - value1.X) * amount,
			value1.Y + (value2.Y - value1.Y) * amount,
			value1.Z + (value2.Z - value1.Z) * amount);
	}

	/// Adds two vectors together.
	/// @param left The first source vector.
	/// @param right The second source vector.
	/// @return The summed vector.
	static inline Vector3 Add(const Vector3& left, const Vector3& right)
	{
		return left + right;
	}

	/// Subtracts the second vector from the first.
	/// @param left The first source vector.
	/// @param right The second source vector.
	/// @return The difference vector.
	static inline Vector3 Subtract(const Vector3& left, const Vector3& right)
	{
		return left - right;
	}

	/// Multiplies two vectors together.
	/// @param left The first source vector.
	/// @param right The second source vector.
	/// @return The product vector.
	static inline Vector3 Multiply(const Vector3& left, const Vector3& right)
	{
		return left * right;
	}

	/// Multiplies a vector by the given scalar.
	/// @param left The source vector.
	/// @param right The scalar value.
	/// @return The scaled vector.
	static inline Vector3 Multiply(const Vector3& left, value_type right)
	{
		return left * right;
	}

	/// Multiplies a vector by the given scalar.
	/// @param left The scalar value.
	/// @param right The source vector.
	/// @return The scaled vector.
	static inline Vector3 Multiply(value_type left, const Vector3& right)
	{
		return Vector3(left) * right;
	}

	/// Divides the first vector by the second.
	/// @param left The first source vector.
	/// @param right The second source vector.
	/// @return The vector resulting from the division.
	static inline Vector3 Divide(const Vector3& left, const Vector3& right)
	{
		return left / right;
	}

	/// Divides the vector by the given scalar.
	/// @param left The source vector.
	/// @param divisor The scalar value.
	/// @return The result of the division.
	static inline Vector3 Divide(const Vector3& left, value_type divisor)
	{
		return left / divisor;
	}

	/// Negates a given vector.
	/// @param value The source vector.
	/// @return The negated vector.
	static inline Vector3 Negate(const Vector3& value)
	{
		return -value;
	}

	/// Returns the dot product of two vectors.
	/// @param vector1 The first vector.
	/// @param vector2 The second vector.
	/// @return The dot product.
	static inline value_type Dot(const Vector3& vector1, const Vector3& vector2)
	{
		return vector1.X * vector2.X +
			vector1.Y * vector2.Y +
			vector1.Z * vector2.Z;
	}

	/// Returns a vector whose elements are the minimum of each of the pairs of elements in the two source vectors.
	/// @param value1 The first source vector.
	/// @param value2 The second source vector.
	/// @return The minimized vector.
	static inline Vector3 Min(const Vector3& value1, const Vector3& value2)
	{
		return Vector3(
			(value1.X < value2.X) ? value1.X : value2.X,
			(value1.Y < value2.Y) ? value1.Y : value2.Y,
			(value1.Z < value2.Z) ? value1.Z : value2.Z);
	}

	/// Returns a vector whose elements are the maximum of each of the pairs of elements in the two source vectors.
	/// @param value1 The first source vector.
	/// @param value2 The second source vector.
	/// @return The maximized vector.
	static inline Vector3 Max(const Vector3& value1, const Vector3& value2)
	{
		return Vector3(
			(value1.X > value2.X) ? value1.X : value2.X,
			(value1.Y > value2.Y) ? value1.Y : value2.Y,
			(value1.Z > value2.Z) ? value1.Z : value2.Z);
	}

	/// Returns a vector whose elements are the absolute values of each of the source vector's elements.
	/// @param value The source vector.
	/// @return The absolute value vector.
	static Vector3 Abs(const Vector3& value)
	{
		return Vector3(Math::Abs(value.X), Math::Abs(value.Y), Math::Abs(value.Z));
	}

	/// Returns a vector whose elements are the square root of each of the source vector's elements.
	/// @param value The source vector.
	/// @return The square root vector
	static Vector3 SquareRoot(const Vector3& value)
	{
		return Vector3(Math::Sqrt(value.X), Math::Sqrt(value.Y), Math::Sqrt(value.Z));
	}


public: /// operator override

    /// Adds two vectors together.
    /// @param right The source vector.
    /// @return The summed vector.
    inline Vector3 operator +(const Vector3&  right)const
    {
        return Vector3(this->X + right.X, this->Y + right.Y, this->Z + right.Z);
    }

    /// Subtracts the second vector from the first.
    /// @param right The source vector.
    /// @return The difference vector.
     inline Vector3 operator -(const Vector3&  right)const
    {
        return Vector3(this->X - right.X, this->Y - right.Y, this->Z - right.Z);
    }

    /// Multiplies two vectors together.
    /// @param right The source vector.
    /// @return The product vector.
    inline Vector3 operator *(const Vector3&  right)const
    {
        return Vector3(this->X * right.X, this->Y * right.Y, this->Z * right.Z);
    }

    /// Multiplies a vector by the given scalar.
    /// @param right The scalar value.
    /// @return The scaled vector.
    inline Vector3 operator *(value_type right)const
    {
        return (*this) * Vector3(right);
    }

    /// Divides the first vector by the second.
    /// @param right The source vector.
    /// @return The vector resulting from the division.
    inline Vector3 operator /(const Vector3&  right)const
    {
        return Vector3(this->X / right.X, this->Y / right.Y, this->Z / right.Z);
    }

    /// Divides the vector by the given scalar.
    /// @param right The scalar value.
    /// @return The result of the division.
    inline Vector3 operator /(value_type right)const
    {
        Vector3::value_type invDiv = 1.0f / right;
        return Vector3(
            this->X * invDiv,
            this->Y * invDiv,
            this->Z * invDiv);
    }

    /// Negates a given vector.
    /// @return The negated vector.
    inline Vector3 operator -()const
    {
        return Vector3::ZERO - (*this);
    }

    /// Returns a boolean indicating whether the two given vectors are equal.
    /// @param right The vector to compare.
    /// @return True if the vectors are equal; False otherwise.
    inline bool operator ==(const Vector3& right)const
    {
        return this->Equals(right);
    }

    /// Returns a boolean indicating whether the two given vectors are not equal.
    /// @param right The vector to compare.
    /// @return True if the vectors are not equal; False if they are equal.
    inline bool operator !=(const Vector3& right)const
    {
        return !((*this) == right);
    }
};

//
///// Adds two vectors together.
///// @param left The first source vector.
///// @param right The second source vector.
///// @return The summed vector.
//static inline Vector3 operator +(const Vector3&  left, const Vector3&  right)
//{
//	return Vector3(left.X + right.X, left.Y + right.Y, left.Z + right.Z);
//}
//
///// Subtracts the second vector from the first.
///// @param left The first source vector.
///// @param right The second source vector.
///// @return The difference vector.
//static  inline Vector3 operator -(const Vector3&  left, const Vector3&  right)
//{
//	return Vector3(left.X - right.X, left.Y - right.Y, left.Z - right.Z);
//}
//
///// Multiplies two vectors together.
///// @param left The first source vector.
///// @param right The second source vector.
///// @return The product vector.
//static inline Vector3 operator *(const Vector3&  left, const Vector3&  right)
//{
//	return Vector3(left.X * right.X, left.Y * right.Y, left.Z * right.Z);
//}
//
///// Multiplies a vector by the given scalar.
///// @param left The source vector.
///// @param right The scalar value.
///// @return The scaled vector.
//static inline Vector3 operator *(const Vector3&  left, Vector3::value_type right)
//{
//	return left * Vector3(right);
//}
//
///// Multiplies a vector by the given scalar.
///// @param left The scalar value.
///// @param right The source vector.
///// @return The scaled vector.
//static inline Vector3 operator *(Vector3::value_type left, const Vector3&  right)
//{
//	return Vector3(left) * right;
//}
//
///// Divides the first vector by the second.
///// @param left The first source vector.
///// @param right The second source vector.
///// @return The vector resulting from the division.
//static inline Vector3 operator /(const Vector3&  left, const Vector3&  right)
//{
//	return Vector3(left.X / right.X, left.Y / right.Y, left.Z / right.Z);
//}
//
//
///// Divides the vector by the given scalar.
///// @param value1 The source vector.
///// @param value2 The scalar value.
///// @return The result of the division.
//static inline Vector3 operator /(const Vector3&  value1, Vector3::value_type value2)
//{
//    Vector3::value_type invDiv = 1.0f / value2;
//
//	return Vector3(
//		value1.X * invDiv,
//		value1.Y * invDiv,
//		value1.Z * invDiv);
//}
//
///// Negates a given vector.
///// @param value The source vector.
///// @return The negated vector.
//static inline Vector3 operator -(const Vector3&  value)
//{
//	return Vector3::ZERO - value;
//}
//
///// Returns a boolean indicating whether the two given vectors are equal.
///// @param left The first vector to compare.
///// @param right The second vector to compare.
///// @return True if the vectors are equal; False otherwise.
//static inline bool operator ==(const Vector3&  left, const Vector3&  right)
//{
//	return (left.X == right.X &&
//		left.Y == right.Y &&
//		left.Z == right.Z);
//}
//
///// Returns a boolean indicating whether the two given vectors are not equal.
///// @param left The first vector to compare.
///// @param right The second vector to compare.
///// @return True if the vectors are not equal; False if they are equal.
//static inline bool operator !=(const Vector3&  left, const Vector3&  right)
//{
//	return (left.X != right.X ||
//		left.Y != right.Y ||
//		left.Z != right.Z);
//}

NS_FK_END
#endif // END OF FOUNDATIONKIT_VECTOR3_HPP
