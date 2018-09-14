/****************************************************************************
Copyright (c) 2018 libo All rights reserved.

losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_QUATERNION_HPP
#define FOUNDATIONKIT_QUATERNION_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <string>
#include "FoundationKit/GenericPlatformMacros.hpp"
#include "FoundationKit/Math/Vector3.hpp"
#include "FoundationKit/Math/Math.hpp"
#include "FoundationKit/Foundation/StringUtils.hpp"

NS_FK_BEGIN

struct Matrix4x4;
/// A structure encapsulating a four-dimensional vector (x,y,z,w), 
/// which is used to efficiently rotate an object about the (x,y,z) vector by the angle theta, where w = cos(theta/2).
struct Quaternion
{
	typedef float value_type;

	/// Specifies the X-value of the vector component of the Quaternion.
	value_type X;
	/// Specifies the Y-value of the vector component of the Quaternion.
	value_type Y;
	/// Specifies the Z-value of the vector component of the Quaternion.
	value_type Z;
	/// Specifies the rotation component of the Quaternion.
	value_type W;

	/// Returns a Quaternion representing no rotation. 
	static Quaternion Identity;

	Quaternion()
		: X(0.0f)
		, Y(0.0f)
		, Z(0.0f)
		, W(0.0f)
	{

	}

	/// Constructs a Quaternion from the given components.
	/// @param x The X component of the Quaternion.
	/// @param y The Y component of the Quaternion.
	/// @param z The Z component of the Quaternion.
	/// @param w The W component of the Quaternion.
	Quaternion(value_type x, value_type y, value_type z, value_type w)
		: X(x)
		, Y(y)
		, Z(z)
		, W(w)
	{

	}

	/// Constructs a Quaternion from the given vector and rotation parts.
	/// @param vectorPart The vector part of the Quaternion.
	/// @param scalarPart The rotation part of the Quaternion.
	Quaternion(const Vector3& vectorPart, value_type scalarPart)
	{
		X = vectorPart.X;
		Y = vectorPart.Y;
		Z = vectorPart.Z;
		W = scalarPart;
	}

	/// Returns a boolean indicating whether the given Quaternion is equal to this Quaternion instance.
	/// @param other The Quaternion to compare this instance to.
	/// @return True if the other Quaternion is equal to this instance; False otherwise.
	bool Equals(const Quaternion& other)const
	{
		return (X == other.X &&
			Y == other.Y &&
			Z == other.Z &&
			W == other.W);
	}

	/// Returns a String representing this Quaternion instance.
	/// @return The string representation.
	std::string ToString()const
	{
		return StringUtils::Format("[X:%f Y:%f Z:%f W:%f]", X, Y, Z, W);
	}

	/// Returns the hash code for this instance.
	/// @return The hash code.
	size_t GetHashCode()const
	{
		auto hasher = std::hash<value_type>{};
		return(hasher(X) + hasher(Y) + hasher(Z) + hasher(W));
	}

	/// Returns whether the Quaternion is the identity Quaternion.
	bool IsIdentity()const
	{
		return (X == 0.f && Y == 0.f && Z == 0.f && W == 1.f);
	}

	/// Calculates the length of the Quaternion.
	/// @return The computed length of the Quaternion.
	value_type Length()const
	{
		value_type ls = X * X + Y * Y + Z * Z + W * W;
		return (value_type)Math::Sqrt((double)ls);
	}

	/// Calculates the length squared of the Quaternion. This operation is cheaper than Length().
	/// @return The length squared of the Quaternion.
	value_type LengthSquared()const
	{
		return X * X + Y * Y + Z * Z + W * W;
	}

	value_type* Ptr()
	{
		return reinterpret_cast<value_type*>(this);
	}

public: /// static functions

	/// Divides each component of the Quaternion by the length of the Quaternion.
	/// @param value The source Quaternion.
	/// @return The normalized Quaternion.
	static Quaternion Normalize(const Quaternion& value)
	{
		Quaternion ans;
		value_type ls = value.X * value.X + value.Y * value.Y + value.Z * value.Z + value.W * value.W;
		value_type invNorm = 1.0f / (value_type)Math::Sqrt((double)ls);
		ans.X = value.X * invNorm;
		ans.Y = value.Y * invNorm;
		ans.Z = value.Z * invNorm;
		ans.W = value.W * invNorm;
		return ans;
	}

	/// Creates the conjugate of a specified Quaternion.
	/// @param value The Quaternion of which to return the conjugate.
	/// @return A new Quaternion that is the conjugate of the specified one.
	static Quaternion Conjugate(const Quaternion& value)
	{
		Quaternion ans;
		ans.X = -value.X;
		ans.Y = -value.Y;
		ans.Z = -value.Z;
		ans.W = value.W;
		return ans;
	}

	/// Returns the inverse of a Quaternion.
	/// @param value The source Quaternion.
	/// @return The inverted Quaternion.
	static Quaternion Inverse(const Quaternion& value)
	{
		//  -1   (       a              -v       )
		// q   = ( -------------   ------------- )
		//       (  a^2 + |v|^2  ,  a^2 + |v|^2  )

		Quaternion ans;
		value_type ls = value.X * value.X + value.Y * value.Y + value.Z * value.Z + value.W * value.W;
		value_type invNorm = 1.0f / ls;
		ans.X = -value.X * invNorm;
		ans.Y = -value.Y * invNorm;
		ans.Z = -value.Z * invNorm;
		ans.W = value.W * invNorm;
		return ans;
	}

	/// Creates a Quaternion from a vector and an angle to rotate about the vector.
	/// @param axis The vector to rotate around.
	/// @param angle The angle, in radians, to rotate around the vector.
	/// @return The created Quaternion.
	static Quaternion CreateFromAxisAngle(const Vector3& axis, value_type angle)
	{
		Quaternion ans;

		value_type halfAngle = angle * 0.5f;
		value_type s = (value_type)Math::Sin(halfAngle);
		value_type c = (value_type)Math::Cos(halfAngle);

		ans.X = axis.X * s;
		ans.Y = axis.Y * s;
		ans.Z = axis.Z * s;
		ans.W = c;

		return ans;
	}

	/// Creates a new Quaternion from the given yaw, pitch, and roll, in radians.
	/// @param yaw The yaw angle, in radians, around the Y-axis.
	/// @param pitch The pitch angle, in radians, around the X-axis.
	/// @param roll The roll angle, in radians, around the Z-axis.
	/// @return Quaternion
	static Quaternion CreateFromYawPitchRoll(value_type yaw, value_type pitch, value_type roll)
	{
		//  Roll first, about axis the object is facing, then
		//  pitch upward, then yaw to face into the new heading
		value_type sr, cr, sp, cp, sy, cy;

		value_type halfRoll = roll * 0.5f;
		sr = (value_type)Math::Sin(halfRoll);
		cr = (value_type)Math::Cos(halfRoll);

		value_type halfPitch = pitch * 0.5f;
		sp = (value_type)Math::Sin(halfPitch);
		cp = (value_type)Math::Cos(halfPitch);

		value_type halfYaw = yaw * 0.5f;
		sy = (value_type)Math::Sin(halfYaw);
		cy = (value_type)Math::Cos(halfYaw);

		Quaternion result;

		result.X = cy * sp * cr + sy * cp * sr;
		result.Y = sy * cp * cr - cy * sp * sr;
		result.Z = cy * cp * sr - sy * sp * cr;
		result.W = cy * cp * cr + sy * sp * sr;

		return result;
	}

	/// Creates a Quaternion from the given rotation matrix.
	/// @param matrix The rotation matrix.
	/// @return The created Quaternion.
	static Quaternion CreateFromRotationMatrix(const Matrix4x4& matrix);


	/// Calculates the dot product of two Quaternions.
	/// @param quaternion1 The first source Quaternion.
	/// @param quaternion2 The second source Quaternion.
	/// @return The dot product of the Quaternions.
	static value_type Dot(const Quaternion& quaternion1, const Quaternion& quaternion2)
	{
		return quaternion1.X * quaternion2.X +
			quaternion1.Y * quaternion2.Y +
			quaternion1.Z * quaternion2.Z +
			quaternion1.W * quaternion2.W;
	}

	/// Interpolates between two quaternions, using spherical linear interpolation.
	/// @param quaternion1 The first source Quaternion.
	/// @param quaternion2 The second source Quaternion.
	/// @param amount The relative weight of the second source Quaternion in the interpolation.
	/// @return The interpolated Quaternion.
	static Quaternion Slerp(const Quaternion& quaternion1, const Quaternion& quaternion2, value_type amount)
	{
		const value_type epsilon = 1e-6f;

		value_type t = amount;

		value_type cosOmega = quaternion1.X * quaternion2.X + quaternion1.Y * quaternion2.Y +
			quaternion1.Z * quaternion2.Z + quaternion1.W * quaternion2.W;

		bool flip = false;

		if (cosOmega < 0.0f)
		{
			flip = true;
			cosOmega = -cosOmega;
		}

		value_type s1, s2;

		if (cosOmega > (1.0f - epsilon))
		{
			// Too close, do straight linear interpolation.
			s1 = 1.0f - t;
			s2 = (flip) ? -t : t;
		}
		else
		{
			value_type omega = Math::Acos(cosOmega);
			value_type invSinOmega = (value_type)(1 / Math::Sin(omega));

			s1 = (value_type)Math::Sin((1.0f - t) * omega) * invSinOmega;
			s2 = (flip)
				? (value_type)-Math::Sin(t * omega) * invSinOmega
				: (value_type)Math::Sin(t * omega) * invSinOmega;
		}

		Quaternion ans;

		ans.X = s1 * quaternion1.X + s2 * quaternion2.X;
		ans.Y = s1 * quaternion1.Y + s2 * quaternion2.Y;
		ans.Z = s1 * quaternion1.Z + s2 * quaternion2.Z;
		ans.W = s1 * quaternion1.W + s2 * quaternion2.W;

		return ans;
	}

	///  Linearly interpolates between two quaternions.
	/// @param quaternion1 The first source Quaternion.
	/// @param quaternion2 The second source Quaternion.
	/// @param amount The relative weight of the second source Quaternion in the interpolation.
	/// @return The interpolated Quaternion.
	static Quaternion Lerp(const Quaternion& quaternion1, const Quaternion& quaternion2, value_type amount)
	{
		value_type t = amount;
		value_type t1 = 1.0f - t;

		Quaternion r;

		value_type dot = quaternion1.X * quaternion2.X + quaternion1.Y * quaternion2.Y +
			quaternion1.Z * quaternion2.Z + quaternion1.W * quaternion2.W;

		if (dot >= 0.0f)
		{
			r.X = t1 * quaternion1.X + t * quaternion2.X;
			r.Y = t1 * quaternion1.Y + t * quaternion2.Y;
			r.Z = t1 * quaternion1.Z + t * quaternion2.Z;
			r.W = t1 * quaternion1.W + t * quaternion2.W;
		}
		else
		{
			r.X = t1 * quaternion1.X - t * quaternion2.X;
			r.Y = t1 * quaternion1.Y - t * quaternion2.Y;
			r.Z = t1 * quaternion1.Z - t * quaternion2.Z;
			r.W = t1 * quaternion1.W - t * quaternion2.W;
		}

		// Normalize it.
		value_type ls = r.X * r.X + r.Y * r.Y + r.Z * r.Z + r.W * r.W;
		value_type invNorm = 1.0f / (value_type)Math::Sqrt((double)ls);

		r.X *= invNorm;
		r.Y *= invNorm;
		r.Z *= invNorm;
		r.W *= invNorm;

		return r;
	}

	/// Concatenates two Quaternions; the result represents the value1 rotation followed by the value2 rotation.
	/// @param value1 The first Quaternion rotation in the series.
	/// @param value2 The second Quaternion rotation in the series.
	/// @return A new Quaternion representing the concatenation of the value1 rotation followed by the value2 rotation.
	static Quaternion Concatenate(const Quaternion& value1, const Quaternion& value2)
	{
		Quaternion ans;

		// Concatenate rotation is actually q2 * q1 instead of q1 * q2.
		// So that's why value2 goes q1 and value1 goes q2.
		value_type q1x = value2.X;
		value_type q1y = value2.Y;
		value_type q1z = value2.Z;
		value_type q1w = value2.W;

		value_type q2x = value1.X;
		value_type q2y = value1.Y;
		value_type q2z = value1.Z;
		value_type q2w = value1.W;

		// cross(av, bv)
		value_type cx = q1y * q2z - q1z * q2y;
		value_type cy = q1z * q2x - q1x * q2z;
		value_type cz = q1x * q2y - q1y * q2x;

		value_type dot = q1x * q2x + q1y * q2y + q1z * q2z;

		ans.X = q1x * q2w + q2x * q1w + cx;
		ans.Y = q1y * q2w + q2y * q1w + cy;
		ans.Z = q1z * q2w + q2z * q1w + cz;
		ans.W = q1w * q2w - dot;

		return ans;
	}

	/// Flips the sign of each component of the quaternion.
	/// @param value The source Quaternion.
	/// @return The negated Quaternion.
	static Quaternion Negate(const Quaternion& value)
	{
		Quaternion ans;

		ans.X = -value.X;
		ans.Y = -value.Y;
		ans.Z = -value.Z;
		ans.W = -value.W;

		return ans;
	}

	/// Adds two Quaternions element-by-element.
	/// @param value1 The first source Quaternion.
	/// @param value2 The second source Quaternion.
	/// @return The result of adding the Quaternions.
	static Quaternion Add(const Quaternion& value1, const Quaternion& value2)
	{
		Quaternion ans;

		ans.X = value1.X + value2.X;
		ans.Y = value1.Y + value2.Y;
		ans.Z = value1.Z + value2.Z;
		ans.W = value1.W + value2.W;

		return ans;
	}

	/// Subtracts one Quaternion from another.
	/// @param value1 The first source Quaternion.
	/// @param value2 The second Quaternion, to be subtracted from the first.
	/// @return The result of the subtraction.
	static Quaternion Subtract(const Quaternion& value1, const Quaternion& value2)
	{
		Quaternion ans;

		ans.X = value1.X - value2.X;
		ans.Y = value1.Y - value2.Y;
		ans.Z = value1.Z - value2.Z;
		ans.W = value1.W - value2.W;

		return ans;
	}

	/// Multiplies two Quaternions together.
	/// @param value1 The Quaternion on the left side of the multiplication.
	/// @param value2 The Quaternion on the right side of the multiplication.
	/// @return The result of the multiplication.
	static Quaternion Multiply(const Quaternion& value1, const Quaternion& value2)
	{
		Quaternion ans;

		value_type q1x = value1.X;
		value_type q1y = value1.Y;
		value_type q1z = value1.Z;
		value_type q1w = value1.W;

		value_type q2x = value2.X;
		value_type q2y = value2.Y;
		value_type q2z = value2.Z;
		value_type q2w = value2.W;

		// cross(av, bv)
		value_type cx = q1y * q2z - q1z * q2y;
		value_type cy = q1z * q2x - q1x * q2z;
		value_type cz = q1x * q2y - q1y * q2x;

		value_type dot = q1x * q2x + q1y * q2y + q1z * q2z;

		ans.X = q1x * q2w + q2x * q1w + cx;
		ans.Y = q1y * q2w + q2y * q1w + cy;
		ans.Z = q1z * q2w + q2z * q1w + cz;
		ans.W = q1w * q2w - dot;

		return ans;
	}

	/// Multiplies a Quaternion by a scalar value.
	/// @param value1 The source Quaternion.
	/// @param value2 The scalar value.
	/// @return The result of the multiplication.
	static Quaternion Multiply(const Quaternion& value1, value_type value2)
	{
		Quaternion ans;

		ans.X = value1.X * value2;
		ans.Y = value1.Y * value2;
		ans.Z = value1.Z * value2;
		ans.W = value1.W * value2;

		return ans;
	}

	/// Divides a Quaternion by another Quaternion.
	/// @param value1 The source Quaternion.
	/// @param value2 The divisor.
	/// @return The result of the division.
	static Quaternion Divide(const Quaternion& value1, const Quaternion& value2)
	{
		Quaternion ans;

		value_type q1x = value1.X;
		value_type q1y = value1.Y;
		value_type q1z = value1.Z;
		value_type q1w = value1.W;

		//-------------------------------------
		// Inverse part.
		value_type ls = value2.X * value2.X + value2.Y * value2.Y +
			value2.Z * value2.Z + value2.W * value2.W;
		value_type invNorm = 1.0f / ls;

		value_type q2x = -value2.X * invNorm;
		value_type q2y = -value2.Y * invNorm;
		value_type q2z = -value2.Z * invNorm;
		value_type q2w = value2.W * invNorm;

		//-------------------------------------
		// Multiply part.

		// cross(av, bv)
		value_type cx = q1y * q2z - q1z * q2y;
		value_type cy = q1z * q2x - q1x * q2z;
		value_type cz = q1x * q2y - q1y * q2x;

		value_type dot = q1x * q2x + q1y * q2y + q1z * q2z;

		ans.X = q1x * q2w + q2x * q1w + cx;
		ans.Y = q1y * q2w + q2y * q1w + cy;
		ans.Z = q1z * q2w + q2z * q1w + cz;
		ans.W = q1w * q2w - dot;

		return ans;
	}

public: /// operator override

	/// Flips the sign of each component of the quaternion.
	/// @return The negated Quaternion.
	inline Quaternion operator -()const
	{
		Quaternion ans;
		ans.X = -this->X;
		ans.Y = -this->Y;
		ans.Z = -this->Z;
		ans.W = -this->W;
		return ans;
	}

	/// Adds two Quaternions element-by-element.
	/// @param right The source Quaternion.
	/// @return The result of adding the Quaternions.
	inline Quaternion operator +(const Quaternion& right)const
	{
		Quaternion ans;

		ans.X = this->X + right.X;
		ans.Y = this->Y + right.Y;
		ans.Z = this->Z + right.Z;
		ans.W = this->W + right.W;

		return ans;
	}

	/// Subtracts one Quaternion from another.
	/// @param right The Quaternion, to be subtracted from this.
	/// @return The result of the subtraction.
	inline Quaternion operator -(const Quaternion& right)const
	{
		Quaternion ans;
		ans.X = this->X - right.X;
		ans.Y = this->Y - right.Y;
		ans.Z = this->Z - right.Z;
		ans.W = this->W - right.W;
		return ans;
	}

	/// Multiplies two Quaternions together.
	/// @param right The Quaternion on the right side of the multiplication.
	/// @return The result of the multiplication.
	inline Quaternion operator *(const Quaternion& right)const
	{
		Quaternion ans;

		value_type q1x = this->X;
		value_type q1y = this->Y;
		value_type q1z = this->Z;
		value_type q1w = this->W;

		value_type q2x = right.X;
		value_type q2y = right.Y;
		value_type q2z = right.Z;
		value_type q2w = right.W;

		// cross(av, bv)
		value_type cx = q1y * q2z - q1z * q2y;
		value_type cy = q1z * q2x - q1x * q2z;
		value_type cz = q1x * q2y - q1y * q2x;

		value_type dot = q1x * q2x + q1y * q2y + q1z * q2z;

		ans.X = q1x * q2w + q2x * q1w + cx;
		ans.Y = q1y * q2w + q2y * q1w + cy;
		ans.Z = q1z * q2w + q2z * q1w + cz;
		ans.W = q1w * q2w - dot;

		return ans;
	}

	/// Multiplies a Quaternion by a scalar value.
	/// @param right The scalar value.
	/// @return The result of the multiplication.
	inline Quaternion operator *(value_type right)const
	{
		Quaternion ans;

		ans.X = this->X * right;
		ans.Y = this->Y * right;
		ans.Z = this->Z * right;
		ans.W = this->W * right;

		return ans;
	}

	/// Divides a Quaternion by another Quaternion.
	/// @param right The divisor.
	/// @return The result of the division.
	inline Quaternion operator /(const Quaternion& right)const
	{
		Quaternion ans;

		value_type q1x = this->X;
		value_type q1y = this->Y;
		value_type q1z = this->Z;
		value_type q1w = this->W;

		//-------------------------------------
		// Inverse part.
		value_type ls = right.X * right.X + right.Y * right.Y +
			right.Z * right.Z + right.W * right.W;
		value_type invNorm = 1.0f / ls;

		value_type q2x = -right.X * invNorm;
		value_type q2y = -right.Y * invNorm;
		value_type q2z = -right.Z * invNorm;
		value_type q2w = right.W * invNorm;

		//-------------------------------------
		// Multiply part.

		// cross(av, bv)
		value_type cx = q1y * q2z - q1z * q2y;
		value_type cy = q1z * q2x - q1x * q2z;
		value_type cz = q1x * q2y - q1y * q2x;

		value_type dot = q1x * q2x + q1y * q2y + q1z * q2z;

		ans.X = q1x * q2w + q2x * q1w + cx;
		ans.Y = q1y * q2w + q2y * q1w + cy;
		ans.Z = q1z * q2w + q2z * q1w + cz;
		ans.W = q1w * q2w - dot;

		return ans;
	}

	/// Returns a boolean indicating whether the two given Quaternions are equal.
	/// @param right The Quaternion to compare.
	/// @return True if the Quaternions are equal; False otherwise.
	inline bool operator ==(const Quaternion& right)const
	{
		return this->Equals(right);
	}

	/// Returns a boolean indicating whether the two given Quaternions are not equal.
	/// @param right The Quaternion to compare.
	/// @return True if the Quaternions are not equal; False if they are equal.
	inline bool operator !=(const Quaternion& right)const
	{
		return !((*this) == right);
	}
};

NS_FK_END

#endif // FOUNDATIONKIT_QUATERNION_HPP






