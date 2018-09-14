/****************************************************************************
Copyright (c) 2018 libo All rights reserved.

losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_MATRIX4X4_HPP
#define FOUNDATIONKIT_MATRIX4X4_HPP

#include "FoundationKit/Math/Math.hpp"
#include "FoundationKit/Foundation/StringUtils.hpp"
#include "FoundationKit/Math/Matrix3x2.hpp"
#include "FoundationKit/Math/Plane.hpp"
#include "FoundationKit/Math/Quaternion.hpp"
#include "FoundationKit/Math/Vector3.hpp"

NS_FK_BEGIN

/// A structure encapsulating a 4x4 matrix.
struct Matrix4x4
{
    typedef float value_type;
	/// Value at row 1, column 1 of the matrix.
	value_type M11;

	/// Value at row 1, column 2 of the matrix.
	value_type M12;

	/// Value at row 1, column 3 of the matrix.
	value_type M13;

	/// Value at row 1, column 4 of the matrix.
	value_type M14;

	/// Value at row 2, column 1 of the matrix.
	value_type M21;

	/// Value at row 2, column 2 of the matrix.
	value_type M22;

	/// Value at row 2, column 3 of the matrix.
	value_type M23;

	/// Value at row 2, column 4 of the matrix.
	value_type M24;

	/// Value at row 3, column 1 of the matrix.
	value_type M31;

	/// Value at row 3, column 2 of the matrix.
	value_type M32;

	/// Value at row 3, column 3 of the matrix.
	value_type M33;

	/// Value at row 3, column 4 of the matrix.
	value_type M34;

	/// Value at row 4, column 1 of the matrix.
	value_type M41;

	/// Value at row 4, column 2 of the matrix.
	value_type M42;

	/// Value at row 4, column 3 of the matrix.
	value_type M43;

	/// Value at row 4, column 4 of the matrix.
	value_type M44;

	static const Matrix4x4 Identity;

	Matrix4x4():
		M11(0.0f),
		M12(0.0f),
		M13(0.0f),
		M14(0.0f),
		M21(0.0f),
		M22(0.0f),
		M23(0.0f),
		M24(0.0f),
		M31(0.0f),
		M32(0.0f),
		M33(0.0f),
		M34(0.0f),
		M41(0.0f),
		M42(0.0f),
		M43(0.0f),
		M44(0.0f)
	{

	}
	/// Constructs a Matrix4x4 from the given components.
	Matrix4x4(value_type m11, value_type m12, value_type m13, value_type m14,
		value_type m21, value_type m22, value_type m23, value_type m24,
		value_type m31, value_type m32, value_type m33, value_type m34,
		value_type m41, value_type m42, value_type m43, value_type m44)
		: M11(m11)
		, M12(m12)
		, M13(m13)
		, M14(m14)
		, M21(m21)
		, M22(m22)
		, M23(m23)
		, M24(m24)
		, M31(m31)
		, M32(m32)
		, M33(m33)
		, M34(m34)
		, M41(m41)
		, M42(m42)
		, M43(m43)
		, M44(m44)
	{

	}

	/// Constructs a Matrix4x4 from the given Matrix3x2.
	/// @param value  The source Matrix3x2.
	Matrix4x4(const Matrix3x2& value)
	{
		M11 = value.M11;
		M12 = value.M12;
		M13 = 0.0f;
		M14 = 0.0f;
		M21 = value.M21;
		M22 = value.M22;
		M23 = 0.0f;
		M24 = 0.0f;
		M31 = 0.0f;
		M32 = 0.0f;
		M33 = 1.0f;
		M34 = 0.0f;
		M41 = value.M31;
		M42 = value.M32;
		M43 = 0.0f;
		M44 = 1.0f;
	}

	/// Returns a boolean indicating whether this matrix instance is equal to the other given matrix.
	/// @param other The matrix to compare this instance to.
	/// @return True if the matrices are equal; False otherwise.
	bool Equals(const Matrix4x4& other)const
	{
		return (M11 == other.M11 && M22 == other.M22 && M33 == other.M33 && M44 == other.M44 && // Check diagonal element first for early out.
			M12 == other.M12 && M13 == other.M13 && M14 == other.M14 &&
			M21 == other.M21 && M23 == other.M23 && M24 == other.M24 &&
			M31 == other.M31 && M32 == other.M32 && M34 == other.M34 &&
			M41 == other.M41 && M42 == other.M42 && M43 == other.M43);
	}

	/// Returns a String representing this matrix instance.
	/// @return The string representation.
	std::string ToString()const
	{
		;
		return StringUtils::Format("[[M11:%f M12:%f M13:%f M14:%f],[M21:%f M22:%f M23:%f M24:%f],[M31:%f M32:%f M33:%f M34:%f],[M41:%f M42:%f M43:%f M44:%f]]",
			M11, M12, M13, M14,
			M21, M22, M23, M24,
			M31, M32, M33, M34,
			M41, M42, M43, M44);
	}


	/// Returns the hash code for this instance.
	/// @return The hash code.
	size_t GetHashCode()const
	{
		auto hasher = std::hash<value_type>{};
		return hasher(M11) + hasher(M12) + hasher(M13) + hasher(M14) +
			hasher(M21) + hasher(M22) + hasher(M23) + hasher(M24) +
			hasher(M31) + hasher(M32) + hasher(M33) + hasher(M34) +
			hasher(M41) + hasher(M42) + hasher(M43) + hasher(M44);
	}


	/// Returns whether the matrix is the identity matrix.
	bool IsIdentity()
	{

		return (M11 == 1.0f && M22 == 1.0f && M33 == 1.0f && M44 == 1.0f && // Check diagonal element first for early out.
			M12 == 0.0f && M13 == 0.0f && M14 == 0.0f &&
			M21 == 0.0f && M23 == 0.0f && M24 == 0.0f &&
			M31 == 0.0f && M32 == 0.0f && M34 == 0.0f &&
			M41 == 0.0f && M42 == 0.0f && M43 == 0.0f);
	}


	/// Gets the translation component of this matrix.
	Vector3 GetTranslation()
	{
		return Vector3(M41, M42, M43);
	}

	/// Sets the translation component of this matrix.
	void SetTranslation(const Vector3& value)
	{
		M41 = value.X;
		M42 = value.Y;
		M43 = value.Z;
	}

    value_type* Ptr()
    {
        return reinterpret_cast<value_type*>(this);
    }

public: /// static functions

	/// Creates a spherical billboard that rotates around a specified object position.
	/// @param objectPosition Position of the object the billboard will rotate around.
	/// @param cameraPosition Position of the camera.
	/// @param cameraUpVector The up vector of the camera.
	/// @param cameraForwardVector The forward vector of the camera.
	/// @return The created billboard matrix
	static Matrix4x4 CreateBillboard(const Vector3& objectPosition, const Vector3& cameraPosition, const Vector3& cameraUpVector, const Vector3& cameraForwardVector)
	{
		const value_type epsilon = 1e-4f;

		Vector3 zaxis = Vector3(
			objectPosition.X - cameraPosition.X,
			objectPosition.Y - cameraPosition.Y,
			objectPosition.Z - cameraPosition.Z);

		value_type norm = zaxis.LengthSquared();

		if (norm < epsilon)
		{
			zaxis = -cameraForwardVector;
		}
		else
		{
			zaxis = Vector3::Multiply(zaxis, 1.0f / (value_type)Math::Sqrt(norm));
		}

		Vector3 xaxis = Vector3::Normalize(Vector3::Cross(cameraUpVector, zaxis));

		Vector3 yaxis = Vector3::Cross(zaxis, xaxis);

		Matrix4x4 result;

		result.M11 = xaxis.X;
		result.M12 = xaxis.Y;
		result.M13 = xaxis.Z;
		result.M14 = 0.0f;
		result.M21 = yaxis.X;
		result.M22 = yaxis.Y;
		result.M23 = yaxis.Z;
		result.M24 = 0.0f;
		result.M31 = zaxis.X;
		result.M32 = zaxis.Y;
		result.M33 = zaxis.Z;
		result.M34 = 0.0f;

		result.M41 = objectPosition.X;
		result.M42 = objectPosition.Y;
		result.M43 = objectPosition.Z;
		result.M44 = 1.0f;

		return result;
	}

	/// Creates a cylindrical billboard that rotates around a specified axis.
	/// @param objectPosition Position of the object the billboard will rotate around.
	/// @param cameraPosition Position of the camera.
	/// @param rotateAxis Axis to rotate the billboard around.
	/// @param cameraForwardVector Forward vector of the camera.
	/// @param objectForwardVector Forward vector of the object.
	/// @return The created billboard matrix.
	static Matrix4x4 CreateConstrainedBillboard(const Vector3& objectPosition, const Vector3& cameraPosition, const Vector3& rotateAxis, const Vector3& cameraForwardVector, const Vector3& objectForwardVector)
	{
		const value_type epsilon = 1e-4f;
		const value_type minAngle = 1.0f - (0.1f * ((value_type)Math::PI / 180.0f)); // 0.1 degrees

																		  // Treat the case when object and camera positions are too close.
		Vector3 faceDir = Vector3(
			objectPosition.X - cameraPosition.X,
			objectPosition.Y - cameraPosition.Y,
			objectPosition.Z - cameraPosition.Z);

		value_type norm = faceDir.LengthSquared();

		if (norm < epsilon)
		{
			faceDir = -cameraForwardVector;
		}
		else
		{
			faceDir = Vector3::Multiply(faceDir, (1.0f / (value_type)Math::Sqrt(norm)));
		}

		Vector3 yaxis = rotateAxis;
		Vector3 xaxis;
		Vector3 zaxis;

		// Treat the case when angle between faceDir and rotateAxis is too close to 0.
		value_type dot = Vector3::Dot(rotateAxis, faceDir);

		if (Math::Abs(dot) > minAngle)
		{
			zaxis = objectForwardVector;

			// Make sure passed values are useful for compute.
			dot = Vector3::Dot(rotateAxis, zaxis);

			if (Math::Abs(dot) > minAngle)
			{
				zaxis = (Math::Abs(rotateAxis.Z) > minAngle) ? Vector3(1, 0, 0) : Vector3(0, 0, -1);
			}

			xaxis = Vector3::Normalize(Vector3::Cross(rotateAxis, zaxis));
			zaxis = Vector3::Normalize(Vector3::Cross(xaxis, rotateAxis));
		}
		else
		{
			xaxis = Vector3::Normalize(Vector3::Cross(rotateAxis, faceDir));
			zaxis = Vector3::Normalize(Vector3::Cross(xaxis, yaxis));
		}

		Matrix4x4 result;
		result.M11 = xaxis.X;
		result.M12 = xaxis.Y;
		result.M13 = xaxis.Z;
		result.M14 = 0.0f;
		result.M21 = yaxis.X;
		result.M22 = yaxis.Y;
		result.M23 = yaxis.Z;
		result.M24 = 0.0f;
		result.M31 = zaxis.X;
		result.M32 = zaxis.Y;
		result.M33 = zaxis.Z;
		result.M34 = 0.0f;
		result.M41 = objectPosition.X;
		result.M42 = objectPosition.Y;
		result.M43 = objectPosition.Z;
		result.M44 = 1.0f;
		return result;
	}

    /// Creates a translation matrix.
    /// @param xPosition The amount to translate on the X-axis.
    /// @param yPosition The amount to translate on the Y-axis.
    /// @param zPosition The amount to translate on the Z-axis.
    /// @return The translation matrix.
    static inline Matrix4x4 CreateTranslation(value_type xPosition, value_type yPosition, value_type zPosition)
    {
        Matrix4x4 result;
        result.M11 = 1.0f;
        result.M12 = 0.0f;
        result.M13 = 0.0f;
        result.M14 = 0.0f;
        result.M21 = 0.0f;
        result.M22 = 1.0f;
        result.M23 = 0.0f;
        result.M24 = 0.0f;
        result.M31 = 0.0f;
        result.M32 = 0.0f;
        result.M33 = 1.0f;
        result.M34 = 0.0f;
        result.M41 = xPosition;
        result.M42 = yPosition;
        result.M43 = zPosition;
        result.M44 = 1.0f;
        return result;
    }

	/// Creates a translation matrix.
	/// @param position The amount to translate in each axis.
	/// @return The translation matrix.
	static inline Matrix4x4 CreateTranslation(const Vector3& position)
	{
        return CreateTranslation(position.X, position.Y, position.Z);
	}



	/// Creates a scaling matrix.
	/// @param xScale Value to scale by on the X-axis.
	/// @param yScale Value to scale by on the Y-axis.
	/// @param zScale Value to scale by on the Z-axis.
	/// @return The scaling matrix.
	static inline Matrix4x4 CreateScale(value_type xScale, value_type yScale, value_type zScale)
	{
		Matrix4x4 result;
		result.M11 = xScale;
		result.M12 = 0.0f;
		result.M13 = 0.0f;
		result.M14 = 0.0f;
		result.M21 = 0.0f;
		result.M22 = yScale;
		result.M23 = 0.0f;
		result.M24 = 0.0f;
		result.M31 = 0.0f;
		result.M32 = 0.0f;
		result.M33 = zScale;
		result.M34 = 0.0f;
		result.M41 = 0.0f;
		result.M42 = 0.0f;
		result.M43 = 0.0f;
		result.M44 = 1.0f;
		return result;
	}

    /// Creates a scaling matrix.
    /// @param scales The vector containing the amount to scale by on each axis.
    /// @return The scaling matrix.
    static inline Matrix4x4 CreateScale(const Vector3& scales)
    {
        return CreateScale(scales.X, scales.Y, scales.Z);
    }

	/// Creates a scaling matrix with a center point.
	/// @param xScale Value to scale by on the X-axis.
	/// @param yScale Value to scale by on the Y-axis.
	/// @param zScale Value to scale by on the Z-axis.
	/// @param centerPoint The center point.
	/// @return The scaling matrix.
	static inline Matrix4x4 CreateScale(value_type xScale, value_type yScale, value_type zScale, const Vector3& centerPoint)
	{
		Matrix4x4 result;

		value_type tx = centerPoint.X * (1 - xScale);
		value_type ty = centerPoint.Y * (1 - yScale);
		value_type tz = centerPoint.Z * (1 - zScale);

		result.M11 = xScale;
		result.M12 = 0.0f;
		result.M13 = 0.0f;
		result.M14 = 0.0f;
		result.M21 = 0.0f;
		result.M22 = yScale;
		result.M23 = 0.0f;
		result.M24 = 0.0f;
		result.M31 = 0.0f;
		result.M32 = 0.0f;
		result.M33 = zScale;
		result.M34 = 0.0f;
		result.M41 = tx;
		result.M42 = ty;
		result.M43 = tz;
		result.M44 = 1.0f;

		return result;
	}



	/// Creates a scaling matrix with a center point.
	/// @param scales The vector containing the amount to scale by on each axis.
	/// @param centerPoint The center point.
	/// @return The scaling matrix.
	static inline Matrix4x4 CreateScale(const Vector3& scales, const Vector3& centerPoint)
	{
        return CreateScale(scales.X, scales.Y, scales.Z, centerPoint);
	}

	/// Creates a uniform scaling matrix that scales equally on each axis.
	/// @param scale The uniform scaling factor.
	/// @return The scaling matrix.
	static inline Matrix4x4 CreateScale(value_type scale)
	{
		Matrix4x4 result;
		result.M11 = scale;
		result.M12 = 0.0f;
		result.M13 = 0.0f;
		result.M14 = 0.0f;
		result.M21 = 0.0f;
		result.M22 = scale;
		result.M23 = 0.0f;
		result.M24 = 0.0f;
		result.M31 = 0.0f;
		result.M32 = 0.0f;
		result.M33 = scale;
		result.M34 = 0.0f;
		result.M41 = 0.0f;
		result.M42 = 0.0f;
		result.M43 = 0.0f;
		result.M44 = 1.0f;
		return result;
	}

	/// Creates a uniform scaling matrix that scales equally on each axis with a center point.
	/// @param scale The uniform scaling factor.
	/// @param centerPoint The center point.
	/// @return The scaling matrix.
	static inline Matrix4x4 CreateScale(value_type scale, const Vector3& centerPoint)
	{
		Matrix4x4 result;

		value_type tx = centerPoint.X * (1 - scale);
		value_type ty = centerPoint.Y * (1 - scale);
		value_type tz = centerPoint.Z * (1 - scale);

		result.M11 = scale;
		result.M12 = 0.0f;
		result.M13 = 0.0f;
		result.M14 = 0.0f;
		result.M21 = 0.0f;
		result.M22 = scale;
		result.M23 = 0.0f;
		result.M24 = 0.0f;
		result.M31 = 0.0f;
		result.M32 = 0.0f;
		result.M33 = scale;
		result.M34 = 0.0f;
		result.M41 = tx;
		result.M42 = ty;
		result.M43 = tz;
		result.M44 = 1.0f;

		return result;
	}

	/// Creates a matrix for rotating points around the X-axis.
	/// @param radians The amount, in radians, by which to rotate around the X-axis.
	/// @return The rotation matrix.
	static inline Matrix4x4 CreateRotationX(value_type radians)
	{
		Matrix4x4 result;

		value_type c = (value_type)Math::Cos(radians);
		value_type s = (value_type)Math::Sin(radians);

		// [  1  0  0  0 ]
		// [  0  c  s  0 ]
		// [  0 -s  c  0 ]
		// [  0  0  0  1 ]
		result.M11 = 1.0f;
		result.M12 = 0.0f;
		result.M13 = 0.0f;
		result.M14 = 0.0f;
		result.M21 = 0.0f;
		result.M22 = c;
		result.M23 = s;
		result.M24 = 0.0f;
		result.M31 = 0.0f;
		result.M32 = -s;
		result.M33 = c;
		result.M34 = 0.0f;
		result.M41 = 0.0f;
		result.M42 = 0.0f;
		result.M43 = 0.0f;
		result.M44 = 1.0f;

		return result;
	}

	/// Creates a matrix for rotating points around the X-axis, from a center point.
	/// @param radians The amount, in radians, by which to rotate around the X-axis.
	/// @param centerPoint The center point.
	/// @return The rotation matrix.
	static Matrix4x4 CreateRotationX(value_type radians, const Vector3& centerPoint)
	{
		Matrix4x4 result;

		value_type c = (value_type)Math::Cos(radians);
		value_type s = (value_type)Math::Sin(radians);

		value_type y = centerPoint.Y * (1 - c) + centerPoint.Z * s;
		value_type z = centerPoint.Z * (1 - c) - centerPoint.Y * s;

		// [  1  0  0  0 ]
		// [  0  c  s  0 ]
		// [  0 -s  c  0 ]
		// [  0  y  z  1 ]
		result.M11 = 1.0f;
		result.M12 = 0.0f;
		result.M13 = 0.0f;
		result.M14 = 0.0f;
		result.M21 = 0.0f;
		result.M22 = c;
		result.M23 = s;
		result.M24 = 0.0f;
		result.M31 = 0.0f;
		result.M32 = -s;
		result.M33 = c;
		result.M34 = 0.0f;
		result.M41 = 0.0f;
		result.M42 = y;
		result.M43 = z;
		result.M44 = 1.0f;

		return result;
	}

	/// Creates a matrix for rotating points around the Y-axis.
	/// @param radians The amount, in radians, by which to rotate around the Y-axis.
	/// @return The rotation matrix.
	static Matrix4x4 CreateRotationY(value_type radians)
	{
		Matrix4x4 result;

		value_type c = (value_type)Math::Cos(radians);
		value_type s = (value_type)Math::Sin(radians);

		// [  c  0 -s  0 ]
		// [  0  1  0  0 ]
		// [  s  0  c  0 ]
		// [  0  0  0  1 ]
		result.M11 = c;
		result.M12 = 0.0f;
		result.M13 = -s;
		result.M14 = 0.0f;
		result.M21 = 0.0f;
		result.M22 = 1.0f;
		result.M23 = 0.0f;
		result.M24 = 0.0f;
		result.M31 = s;
		result.M32 = 0.0f;
		result.M33 = c;
		result.M34 = 0.0f;
		result.M41 = 0.0f;
		result.M42 = 0.0f;
		result.M43 = 0.0f;
		result.M44 = 1.0f;

		return result;
	}

	/// Creates a matrix for rotating points around the Y-axis, from a center point.
	/// @param radians The amount, in radians, by which to rotate around the Y-axis.
	/// @param centerPoint The center point.
	/// @return The rotation matrix.
	static Matrix4x4 CreateRotationY(value_type radians, const Vector3& centerPoint)
	{
		Matrix4x4 result;

		value_type c = (value_type)Math::Cos(radians);
		value_type s = (value_type)Math::Sin(radians);

		value_type x = centerPoint.X * (1 - c) - centerPoint.Z * s;
		value_type z = centerPoint.Z * (1 - c) + centerPoint.X * s;

		// [  c  0 -s  0 ]
		// [  0  1  0  0 ]
		// [  s  0  c  0 ]
		// [  x  0  z  1 ]
		result.M11 = c;
		result.M12 = 0.0f;
		result.M13 = -s;
		result.M14 = 0.0f;
		result.M21 = 0.0f;
		result.M22 = 1.0f;
		result.M23 = 0.0f;
		result.M24 = 0.0f;
		result.M31 = s;
		result.M32 = 0.0f;
		result.M33 = c;
		result.M34 = 0.0f;
		result.M41 = x;
		result.M42 = 0.0f;
		result.M43 = z;
		result.M44 = 1.0f;

		return result;
	}

	/// Creates a matrix for rotating points around the Z-axis.
	/// @param radians The amount, in radians, by which to rotate around the Z-axis.
	/// @return The rotation matrix.
	static Matrix4x4 CreateRotationZ(value_type radians)
	{
		Matrix4x4 result;

		value_type c = (value_type)Math::Cos(radians);
		value_type s = (value_type)Math::Sin(radians);

		// [  c  s  0  0 ]
		// [ -s  c  0  0 ]
		// [  0  0  1  0 ]
		// [  0  0  0  1 ]
		result.M11 = c;
		result.M12 = s;
		result.M13 = 0.0f;
		result.M14 = 0.0f;
		result.M21 = -s;
		result.M22 = c;
		result.M23 = 0.0f;
		result.M24 = 0.0f;
		result.M31 = 0.0f;
		result.M32 = 0.0f;
		result.M33 = 1.0f;
		result.M34 = 0.0f;
		result.M41 = 0.0f;
		result.M42 = 0.0f;
		result.M43 = 0.0f;
		result.M44 = 1.0f;

		return result;
	}

	/// Creates a matrix for rotating points around the Z-axis, from a center point.
	/// @param radians The amount, in radians, by which to rotate around the Z-axis.
	/// @param centerPoint The center point.
	/// @return The rotation matrix.
	static Matrix4x4 CreateRotationZ(value_type radians, const Vector3& centerPoint)
	{
		Matrix4x4 result;

		value_type c = (value_type)Math::Cos(radians);
		value_type s = (value_type)Math::Sin(radians);

		value_type x = centerPoint.X * (1 - c) + centerPoint.Y * s;
		value_type y = centerPoint.Y * (1 - c) - centerPoint.X * s;

		// [  c  s  0  0 ]
		// [ -s  c  0  0 ]
		// [  0  0  1  0 ]
		// [  x  y  0  1 ]
		result.M11 = c;
		result.M12 = s;
		result.M13 = 0.0f;
		result.M14 = 0.0f;
		result.M21 = -s;
		result.M22 = c;
		result.M23 = 0.0f;
		result.M24 = 0.0f;
		result.M31 = 0.0f;
		result.M32 = 0.0f;
		result.M33 = 1.0f;
		result.M34 = 0.0f;
		result.M41 = x;
		result.M42 = y;
		result.M43 = 0.0f;
		result.M44 = 1.0f;

		return result;
	}

	/// Creates a matrix that rotates around an arbitrary vector.
	/// @param axis The axis to rotate around.
	/// @param angle The angle to rotate around the given axis, in radians.
	/// @return The rotation matrix.
	static Matrix4x4 CreateFromAxisAngle(const Vector3& axis, value_type angle)
	{
		// a: angle
		// x, y, z: unit vector for axis.
		//
		// Rotation matrix M can compute by using below equation.
		//
		//        T               T
		//  M = uu + (cos a)( I-uu ) + (sin a)S
		//
		// Where:
		//
		//  u = ( x, y, z )
		//
		//      [  0 -z  y ]
		//  S = [  z  0 -x ]
		//      [ -y  x  0 ]
		//
		//      [ 1 0 0 ]
		//  I = [ 0 1 0 ]
		//      [ 0 0 1 ]
		//
		//
		//     [  xx+cosa*(1-xx)   yx-cosa*yx-sina*z zx-cosa*xz+sina*y ]
		// M = [ xy-cosa*yx+sina*z    yy+cosa(1-yy)  yz-cosa*yz-sina*x ]
		//     [ zx-cosa*zx-sina*y zy-cosa*zy+sina*x   zz+cosa*(1-zz)  ]
		//
		value_type x = axis.X, y = axis.Y, z = axis.Z;
		value_type sa = (value_type)Math::Sin(angle), ca = (value_type)Math::Cos(angle);
		value_type xx = x * x, yy = y * y, zz = z * z;
		value_type xy = x * y, xz = x * z, yz = y * z;

		Matrix4x4 result;

		result.M11 = xx + ca * (1.0f - xx);
		result.M12 = xy - ca * xy + sa * z;
		result.M13 = xz - ca * xz - sa * y;
		result.M14 = 0.0f;
		result.M21 = xy - ca * xy - sa * z;
		result.M22 = yy + ca * (1.0f - yy);
		result.M23 = yz - ca * yz + sa * x;
		result.M24 = 0.0f;
		result.M31 = xz - ca * xz + sa * y;
		result.M32 = yz - ca * yz - sa * x;
		result.M33 = zz + ca * (1.0f - zz);
		result.M34 = 0.0f;
		result.M41 = 0.0f;
		result.M42 = 0.0f;
		result.M43 = 0.0f;
		result.M44 = 1.0f;

		return result;
	}

	/// Creates a perspective projection matrix based on a field of view, aspect ratio, and near and far view plane distances. 
	/// @param fieldOfView Field of view in the y direction, in radians.
	/// @param aspectRatio Aspect ratio, defined as view space width divided by height.
	/// @param nearPlaneDistance Distance to the near view plane.
	/// @param farPlaneDistance Distance to the far view plane.
	/// @return The perspective projection matrix.
	static Matrix4x4 CreatePerspectiveFieldOfView(value_type fieldOfView, value_type aspectRatio, value_type nearPlaneDistance, value_type farPlaneDistance)
	{
		if (fieldOfView <= 0.0f || fieldOfView >= Math::PI)
			throw std::out_of_range("fieldOfView");

		if (nearPlaneDistance <= 0.0f)
			throw std::out_of_range("nearPlaneDistance");

		if (farPlaneDistance <= 0.0f)
			throw std::out_of_range("farPlaneDistance");

		if (nearPlaneDistance >= farPlaneDistance)
			throw std::out_of_range("nearPlaneDistance");

		value_type yScale = 1.0f / (value_type)Math::Tan(fieldOfView * 0.5f);
		value_type xScale = yScale / aspectRatio;

		Matrix4x4 result;

		result.M11 = xScale;
		result.M12 = result.M13 = result.M14 = 0.0f;

		result.M22 = yScale;
		result.M21 = result.M23 = result.M24 = 0.0f;

		result.M31 = result.M32 = 0.0f;
		result.M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
		result.M34 = -1.0f;

		result.M41 = result.M42 = result.M44 = 0.0f;
		result.M43 = nearPlaneDistance * farPlaneDistance / (nearPlaneDistance - farPlaneDistance);

		return result;
	}

	/// Creates a perspective projection matrix from the given view volume dimensions.
	/// @param width Width of the view volume at the near view plane.
	/// @param height Height of the view volume at the near view plane.
	/// @param nearPlaneDistance Distance to the near view plane.
	/// @param farPlaneDistance Distance to the far view plane.
	/// @return The perspective projection matrix.
	static Matrix4x4 CreatePerspective(value_type width, value_type height, value_type nearPlaneDistance, value_type farPlaneDistance)
	{
		if (nearPlaneDistance <= 0.0f)
			throw std::out_of_range("nearPlaneDistance");

		if (farPlaneDistance <= 0.0f)
			throw std::out_of_range("farPlaneDistance");

		if (nearPlaneDistance >= farPlaneDistance)
			throw std::out_of_range("nearPlaneDistance");

		Matrix4x4 result;

		result.M11 = 2.0f * nearPlaneDistance / width;
		result.M12 = result.M13 = result.M14 = 0.0f;

		result.M22 = 2.0f * nearPlaneDistance / height;
		result.M21 = result.M23 = result.M24 = 0.0f;

		result.M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
		result.M31 = result.M32 = 0.0f;
		result.M34 = -1.0f;

		result.M41 = result.M42 = result.M44 = 0.0f;
		result.M43 = nearPlaneDistance * farPlaneDistance / (nearPlaneDistance - farPlaneDistance);

		return result;
	}

	/// Creates a customized, perspective projection matrix.
	/// @param left Minimum x-value of the view volume at the near view plane.
	/// @param right Maximum x-value of the view volume at the near view plane.
	/// @param bottom Minimum y-value of the view volume at the near view plane.
	/// @param top Maximum y-value of the view volume at the near view plane.
	/// @param nearPlaneDistance Distance to the near view plane.
	/// @param farPlaneDistance Distance to of the far view plane.
	/// @return The perspective projection matrix.
	static Matrix4x4 CreatePerspectiveOffCenter(value_type left, value_type right, value_type bottom, value_type top, value_type nearPlaneDistance, value_type farPlaneDistance)
	{
		if (nearPlaneDistance <= 0.0f)
			throw std::out_of_range("nearPlaneDistance");

		if (farPlaneDistance <= 0.0f)
			throw std::out_of_range("farPlaneDistance");

		if (nearPlaneDistance >= farPlaneDistance)
			throw std::out_of_range("nearPlaneDistance");

		Matrix4x4 result;

		result.M11 = 2.0f * nearPlaneDistance / (right - left);
		result.M12 = result.M13 = result.M14 = 0.0f;

		result.M22 = 2.0f * nearPlaneDistance / (top - bottom);
		result.M21 = result.M23 = result.M24 = 0.0f;

		result.M31 = (left + right) / (right - left);
		result.M32 = (top + bottom) / (top - bottom);
		result.M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
		result.M34 = -1.0f;

		result.M43 = nearPlaneDistance * farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
		result.M41 = result.M42 = result.M44 = 0.0f;

		return result;
	}

	/// Creates an orthographic perspective matrix from the given view volume dimensions.
	/// @param width Width of the view volume.
	/// @param height Height of the view volume.
	/// @param zNearPlane Minimum Z-value of the view volume.
	/// @param zFarPlane Maximum Z-value of the view volume.
	/// @return The orthographic projection matrix.
	static Matrix4x4 CreateOrthographic(value_type width, value_type height, value_type zNearPlane, value_type zFarPlane)
	{
		Matrix4x4 result;

		result.M11 = 2.0f / width;
		result.M12 = result.M13 = result.M14 = 0.0f;

		result.M22 = 2.0f / height;
		result.M21 = result.M23 = result.M24 = 0.0f;

		result.M33 = 1.0f / (zNearPlane - zFarPlane);
		result.M31 = result.M32 = result.M34 = 0.0f;

		result.M41 = result.M42 = 0.0f;
		result.M43 = zNearPlane / (zNearPlane - zFarPlane);
		result.M44 = 1.0f;

		return result;
	}


	/// Builds a customized, orthographic projection matrix.
	/// @param left Minimum X-value of the view volume.
	/// @param right Maximum X-value of the view volume.
	/// @param bottom Minimum Y-value of the view volume.
	/// @param top Maximum Y-value of the view volume.
	/// @param zNearPlane Minimum Z-value of the view volume.
	/// @param zFarPlane Maximum Z-value of the view volume.
	/// @return The orthographic projection matrix.
	static Matrix4x4 CreateOrthographicOffCenter(value_type left, value_type right, value_type bottom, value_type top, value_type zNearPlane, value_type zFarPlane)
	{
		Matrix4x4 result;

		result.M11 = 2.0f / (right - left);
		result.M12 = result.M13 = result.M14 = 0.0f;

		result.M22 = 2.0f / (top - bottom);
		result.M21 = result.M23 = result.M24 = 0.0f;

		result.M33 = 1.0f / (zNearPlane - zFarPlane);
		result.M31 = result.M32 = result.M34 = 0.0f;

		result.M41 = (left + right) / (left - right);
		result.M42 = (top + bottom) / (bottom - top);
		result.M43 = zNearPlane / (zNearPlane - zFarPlane);
		result.M44 = 1.0f;

		return result;
	}


	/// Creates a view matrix.
	/// @param cameraPosition The position of the camera.
	/// @param cameraTarget The target towards which the camera is pointing.
	/// @param cameraUpVector The direction that is "up" from the camera's point of view.
	/// @return The view matrix.
	static Matrix4x4 CreateLookAt(const Vector3& cameraPosition, const Vector3& cameraTarget, const Vector3& cameraUpVector)
	{
		Vector3 zaxis = Vector3::Normalize(cameraPosition - cameraTarget);
		Vector3 xaxis = Vector3::Normalize(Vector3::Cross(cameraUpVector, zaxis));
		Vector3 yaxis = Vector3::Cross(zaxis, xaxis);

		Matrix4x4 result;

		result.M11 = xaxis.X;
		result.M12 = yaxis.X;
		result.M13 = zaxis.X;
		result.M14 = 0.0f;
		result.M21 = xaxis.Y;
		result.M22 = yaxis.Y;
		result.M23 = zaxis.Y;
		result.M24 = 0.0f;
		result.M31 = xaxis.Z;
		result.M32 = yaxis.Z;
		result.M33 = zaxis.Z;
		result.M34 = 0.0f;
		result.M41 = -Vector3::Dot(xaxis, cameraPosition);
		result.M42 = -Vector3::Dot(yaxis, cameraPosition);
		result.M43 = -Vector3::Dot(zaxis, cameraPosition);
		result.M44 = 1.0f;

		return result;
	}


	/// Creates a world matrix with the specified parameters.
	/// @param position The position of the object; used in translation operations.
	/// @param forward Forward direction of the object.
	/// @param up Upward direction of the object; usually [0, 1, 0].
	/// @return The world matrix.
	static Matrix4x4 CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up)
	{
		Vector3 zaxis = Vector3::Normalize(-forward);
		Vector3 xaxis = Vector3::Normalize(Vector3::Cross(up, zaxis));
		Vector3 yaxis = Vector3::Cross(zaxis, xaxis);

		Matrix4x4 result;

		result.M11 = xaxis.X;
		result.M12 = xaxis.Y;
		result.M13 = xaxis.Z;
		result.M14 = 0.0f;
		result.M21 = yaxis.X;
		result.M22 = yaxis.Y;
		result.M23 = yaxis.Z;
		result.M24 = 0.0f;
		result.M31 = zaxis.X;
		result.M32 = zaxis.Y;
		result.M33 = zaxis.Z;
		result.M34 = 0.0f;
		result.M41 = position.X;
		result.M42 = position.Y;
		result.M43 = position.Z;
		result.M44 = 1.0f;

		return result;
	}


	/// Creates a rotation matrix from the given Quaternion rotation value.
	/// @param quaternion The source Quaternion.
	/// @return The rotation matrix.
	static Matrix4x4 CreateFromQuaternion(Quaternion quaternion)
	{
		Matrix4x4 result;

		value_type xx = quaternion.X * quaternion.X;
		value_type yy = quaternion.Y * quaternion.Y;
		value_type zz = quaternion.Z * quaternion.Z;

		value_type xy = quaternion.X * quaternion.Y;
		value_type wz = quaternion.Z * quaternion.W;
		value_type xz = quaternion.Z * quaternion.X;
		value_type wy = quaternion.Y * quaternion.W;
		value_type yz = quaternion.Y * quaternion.Z;
		value_type wx = quaternion.X * quaternion.W;

		result.M11 = 1.0f - 2.0f * (yy + zz);
		result.M12 = 2.0f * (xy + wz);
		result.M13 = 2.0f * (xz - wy);
		result.M14 = 0.0f;
		result.M21 = 2.0f * (xy - wz);
		result.M22 = 1.0f - 2.0f * (zz + xx);
		result.M23 = 2.0f * (yz + wx);
		result.M24 = 0.0f;
		result.M31 = 2.0f * (xz + wy);
		result.M32 = 2.0f * (yz - wx);
		result.M33 = 1.0f - 2.0f * (yy + xx);
		result.M34 = 0.0f;
		result.M41 = 0.0f;
		result.M42 = 0.0f;
		result.M43 = 0.0f;
		result.M44 = 1.0f;

		return result;
	}


	/// Creates a rotation matrix from the specified yaw, pitch, and roll.
	/// @param yaw Angle of rotation, in radians, around the Y-axis.
	/// @param pitch Angle of rotation, in radians, around the X-axis.
	/// @param roll Angle of rotation, in radians, around the Z-axis.
	/// @return The rotation matrix.
	static Matrix4x4 CreateFromYawPitchRoll(value_type yaw, value_type pitch, value_type roll)
	{
		Quaternion q = Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);

		return Matrix4x4::CreateFromQuaternion(q);
	}


	/// Creates a Matrix that flattens geometry into a specified Plane as if casting a shadow from a specified light source.
	/// @param lightDirection The direction from which the light that will cast the shadow is coming.
	/// @param plane The Plane onto which the new matrix should flatten geometry so as to cast a shadow.
	/// @return A new Matrix that can be used to flatten geometry onto the specified plane from the specified direction.
	static Matrix4x4 CreateShadow(const Vector3& lightDirection, const Plane& plane)
	{
		Plane p = Plane::Normalize(plane);

		value_type dot = p.Normal.X * lightDirection.X + p.Normal.Y * lightDirection.Y + p.Normal.Z * lightDirection.Z;
		value_type a = -p.Normal.X;
		value_type b = -p.Normal.Y;
		value_type c = -p.Normal.Z;
		value_type d = -p.D;

		Matrix4x4 result;

		result.M11 = a * lightDirection.X + dot;
		result.M21 = b * lightDirection.X;
		result.M31 = c * lightDirection.X;
		result.M41 = d * lightDirection.X;

		result.M12 = a * lightDirection.Y;
		result.M22 = b * lightDirection.Y + dot;
		result.M32 = c * lightDirection.Y;
		result.M42 = d * lightDirection.Y;

		result.M13 = a * lightDirection.Z;
		result.M23 = b * lightDirection.Z;
		result.M33 = c * lightDirection.Z + dot;
		result.M43 = d * lightDirection.Z;

		result.M14 = 0.0f;
		result.M24 = 0.0f;
		result.M34 = 0.0f;
		result.M44 = dot;

		return result;
	}


	/// Creates a Matrix that reflects the coordinate system about a specified Plane.
	/// @param value The Plane about which to create a reflection.
	/// @return A new matrix expressing the reflection.
	static Matrix4x4 CreateReflection(const Plane& value)
	{
		Plane p = Plane::Normalize(value);

		value_type a = p.Normal.X;
		value_type b = p.Normal.Y;
		value_type c = p.Normal.Z;

		value_type fa = -2.0f * a;
		value_type fb = -2.0f * b;
		value_type fc = -2.0f * c;

		Matrix4x4 result;

		result.M11 = fa * a + 1.0f;
		result.M12 = fb * a;
		result.M13 = fc * a;
		result.M14 = 0.0f;

		result.M21 = fa * b;
		result.M22 = fb * b + 1.0f;
		result.M23 = fc * b;
		result.M24 = 0.0f;

		result.M31 = fa * c;
		result.M32 = fb * c;
		result.M33 = fc * c + 1.0f;
		result.M34 = 0.0f;

		result.M41 = fa * value.D;
		result.M42 = fb * value.D;
		result.M43 = fc * value.D;
		result.M44 = 1.0f;

		return result;
	}


	/// Calculates the determinant of the matrix.
	/// @return The determinant of the matrix.
	value_type GetDeterminant()
	{
		// | a b c d |     | f g h |     | e g h |     | e f h |     | e f g |
		// | e f g h | = a | j k l | - b | i k l | + c | i j l | - d | i j k |
		// | i j k l |     | n o p |     | m o p |     | m n p |     | m n o |
		// | m n o p |
		//
		//   | f g h |
		// a | j k l | = a ( f ( kp - lo ) - g ( jp - ln ) + h ( jo - kn ) )
		//   | n o p |
		//
		//   | e g h |     
		// b | i k l | = b ( e ( kp - lo ) - g ( ip - lm ) + h ( io - km ) )
		//   | m o p |     
		//
		//   | e f h |
		// c | i j l | = c ( e ( jp - ln ) - f ( ip - lm ) + h ( in - jm ) )
		//   | m n p |
		//
		//   | e f g |
		// d | i j k | = d ( e ( jo - kn ) - f ( io - km ) + g ( in - jm ) )
		//   | m n o |
		//
		// Cost of operation
		// 17 adds and 28 muls.
		//
		// add: 6 + 8 + 3 = 17
		// mul: 12 + 16 = 28

		value_type a = M11, b = M12, c = M13, d = M14;
		value_type e = M21, f = M22, g = M23, h = M24;
		value_type i = M31, j = M32, k = M33, l = M34;
		value_type m = M41, n = M42, o = M43, p = M44;

		value_type kp_lo = k * p - l * o;
		value_type jp_ln = j * p - l * n;
		value_type jo_kn = j * o - k * n;
		value_type ip_lm = i * p - l * m;
		value_type io_km = i * o - k * m;
		value_type in_jm = i * n - j * m;

		return a * (f * kp_lo - g * jp_ln + h * jo_kn) -
			b * (e * kp_lo - g * ip_lm + h * io_km) +
			c * (e * jp_ln - f * ip_lm + h * in_jm) -
			d * (e * jo_kn - f * io_km + g * in_jm);
	}


	/// Attempts to calculate the inverse of the given matrix. If successful, result will contain the inverted matrix.
	/// @param matrix The source matrix to invert.
	/// @param result If successful, contains the inverted matrix.
	/// @return True if the source matrix could be inverted; False otherwise.
	static bool Invert(const Matrix4x4& matrix, Matrix4x4& result)
	{
		//                                       -1
		// If you have matrix M, inverse Matrix M   can compute
		//
		//     -1       1      
		//    M   = --------- A
		//            det(M)
		//
		// A is adjugate (adjoint) of M, where,
		//
		//      T
		// A = C
		//
		// C is Cofactor matrix of M, where,
		//           i + j
		// C   = (-1)      * det(M  )
		//  ij                    ij
		//
		//     [ a b c d ]
		// M = [ e f g h ]
		//     [ i j k l ]
		//     [ m n o p ]
		//
		// First Row
		//           2 | f g h |
		// C   = (-1)  | j k l | = + ( f ( kp - lo ) - g ( jp - ln ) + h ( jo - kn ) )
		//  11         | n o p |
		//
		//           3 | e g h |
		// C   = (-1)  | i k l | = - ( e ( kp - lo ) - g ( ip - lm ) + h ( io - km ) )
		//  12         | m o p |
		//
		//           4 | e f h |
		// C   = (-1)  | i j l | = + ( e ( jp - ln ) - f ( ip - lm ) + h ( in - jm ) )
		//  13         | m n p |
		//
		//           5 | e f g |
		// C   = (-1)  | i j k | = - ( e ( jo - kn ) - f ( io - km ) + g ( in - jm ) )
		//  14         | m n o |
		//
		// Second Row
		//           3 | b c d |
		// C   = (-1)  | j k l | = - ( b ( kp - lo ) - c ( jp - ln ) + d ( jo - kn ) )
		//  21         | n o p |
		//
		//           4 | a c d |
		// C   = (-1)  | i k l | = + ( a ( kp - lo ) - c ( ip - lm ) + d ( io - km ) )
		//  22         | m o p |
		//
		//           5 | a b d |
		// C   = (-1)  | i j l | = - ( a ( jp - ln ) - b ( ip - lm ) + d ( in - jm ) )
		//  23         | m n p |
		//
		//           6 | a b c |
		// C   = (-1)  | i j k | = + ( a ( jo - kn ) - b ( io - km ) + c ( in - jm ) )
		//  24         | m n o |
		//
		// Third Row
		//           4 | b c d |
		// C   = (-1)  | f g h | = + ( b ( gp - ho ) - c ( fp - hn ) + d ( fo - gn ) )
		//  31         | n o p |
		//
		//           5 | a c d |
		// C   = (-1)  | e g h | = - ( a ( gp - ho ) - c ( ep - hm ) + d ( eo - gm ) )
		//  32         | m o p |
		//
		//           6 | a b d |
		// C   = (-1)  | e f h | = + ( a ( fp - hn ) - b ( ep - hm ) + d ( en - fm ) )
		//  33         | m n p |
		//
		//           7 | a b c |
		// C   = (-1)  | e f g | = - ( a ( fo - gn ) - b ( eo - gm ) + c ( en - fm ) )
		//  34         | m n o |
		//
		// Fourth Row
		//           5 | b c d |
		// C   = (-1)  | f g h | = - ( b ( gl - hk ) - c ( fl - hj ) + d ( fk - gj ) )
		//  41         | j k l |
		//
		//           6 | a c d |
		// C   = (-1)  | e g h | = + ( a ( gl - hk ) - c ( el - hi ) + d ( ek - gi ) )
		//  42         | i k l |
		//
		//           7 | a b d |
		// C   = (-1)  | e f h | = - ( a ( fl - hj ) - b ( el - hi ) + d ( ej - fi ) )
		//  43         | i j l |
		//
		//           8 | a b c |
		// C   = (-1)  | e f g | = + ( a ( fk - gj ) - b ( ek - gi ) + c ( ej - fi ) )
		//  44         | i j k |
		//
		// Cost of operation
		// 53 adds, 104 muls, and 1 div.
		value_type a = matrix.M11, b = matrix.M12, c = matrix.M13, d = matrix.M14;
		value_type e = matrix.M21, f = matrix.M22, g = matrix.M23, h = matrix.M24;
		value_type i = matrix.M31, j = matrix.M32, k = matrix.M33, l = matrix.M34;
		value_type m = matrix.M41, n = matrix.M42, o = matrix.M43, p = matrix.M44;

		value_type kp_lo = k * p - l * o;
		value_type jp_ln = j * p - l * n;
		value_type jo_kn = j * o - k * n;
		value_type ip_lm = i * p - l * m;
		value_type io_km = i * o - k * m;
		value_type in_jm = i * n - j * m;

		value_type a11 = +(f * kp_lo - g * jp_ln + h * jo_kn);
		value_type a12 = -(e * kp_lo - g * ip_lm + h * io_km);
		value_type a13 = +(e * jp_ln - f * ip_lm + h * in_jm);
		value_type a14 = -(e * jo_kn - f * io_km + g * in_jm);

		value_type det = a * a11 + b * a12 + c * a13 + d * a14;

		if (Math::Abs(det) < Math::Epsilon)
		{
			result = Matrix4x4();
			return false;
		}

		value_type invDet = 1.0f / det;

		result.M11 = a11 * invDet;
		result.M21 = a12 * invDet;
		result.M31 = a13 * invDet;
		result.M41 = a14 * invDet;

		result.M12 = -(b * kp_lo - c * jp_ln + d * jo_kn) * invDet;
		result.M22 = +(a * kp_lo - c * ip_lm + d * io_km) * invDet;
		result.M32 = -(a * jp_ln - b * ip_lm + d * in_jm) * invDet;
		result.M42 = +(a * jo_kn - b * io_km + c * in_jm) * invDet;

		value_type gp_ho = g * p - h * o;
		value_type fp_hn = f * p - h * n;
		value_type fo_gn = f * o - g * n;
		value_type ep_hm = e * p - h * m;
		value_type eo_gm = e * o - g * m;
		value_type en_fm = e * n - f * m;

		result.M13 = +(b * gp_ho - c * fp_hn + d * fo_gn) * invDet;
		result.M23 = -(a * gp_ho - c * ep_hm + d * eo_gm) * invDet;
		result.M33 = +(a * fp_hn - b * ep_hm + d * en_fm) * invDet;
		result.M43 = -(a * fo_gn - b * eo_gm + c * en_fm) * invDet;

		value_type gl_hk = g * l - h * k;
		value_type fl_hj = f * l - h * j;
		value_type fk_gj = f * k - g * j;
		value_type el_hi = e * l - h * i;
		value_type ek_gi = e * k - g * i;
		value_type ej_fi = e * j - f * i;

		result.M14 = -(b * gl_hk - c * fl_hj + d * fk_gj) * invDet;
		result.M24 = +(a * gl_hk - c * el_hi + d * ek_gi) * invDet;
		result.M34 = -(a * fl_hj - b * el_hi + d * ej_fi) * invDet;
		result.M44 = +(a * fk_gj - b * ek_gi + c * ej_fi) * invDet;

		return true;
	}

	struct CanonicalBasis
	{
		Vector3 Row0;
		Vector3 Row1;
		Vector3 Row2;
	};

	struct VectorBasis
	{
		Vector3* Element0;
		Vector3* Element1;
		Vector3* Element2;
    };


	/// Attempts to extract the scale, translation, and rotation components from the given scale/rotation/translation matrix.
	/// If successful, the out parameters will contained the extracted values.
	/// @param matrix The source matrix.
	/// @param scale The scaling component of the transformation matrix.
	/// @param rotation The rotation component of the transformation matrix.
	/// @param translation The translation component of the transformation matrix
	/// @return True if the source matrix was successfully decomposed; False otherwise.
	static bool Decompose(const Matrix4x4& matrix, Vector3& scale, Quaternion& rotation, Vector3& translation)
	{
		bool result = true;
        Vector3* scaleBase = &scale;
        value_type* pfScales = (value_type*)scaleBase;
        const value_type EPSILON = 0.0001f;
        value_type det;

        VectorBasis vectorBasis;
        Vector3** pVectorBasis = (Vector3**)&vectorBasis;

        Matrix4x4 matTemp = Matrix4x4::Identity;
        CanonicalBasis canonicalBasis = CanonicalBasis();
        Vector3* pCanonicalBasis = &canonicalBasis.Row0;

        canonicalBasis.Row0 = Vector3(1.0f, 0.0f, 0.0f);
        canonicalBasis.Row1 = Vector3(0.0f, 1.0f, 0.0f);
        canonicalBasis.Row2 = Vector3(0.0f, 0.0f, 1.0f);

        translation = Vector3(
            matrix.M41,
            matrix.M42,
            matrix.M43);

        pVectorBasis[0] = (Vector3*)&matTemp.M11;
        pVectorBasis[1] = (Vector3*)&matTemp.M21;
        pVectorBasis[2] = (Vector3*)&matTemp.M31;

        *(pVectorBasis[0]) = Vector3(matrix.M11, matrix.M12, matrix.M13);
        *(pVectorBasis[1]) = Vector3(matrix.M21, matrix.M22, matrix.M23);
        *(pVectorBasis[2]) = Vector3(matrix.M31, matrix.M32, matrix.M33);

        scale.X = pVectorBasis[0]->Length();
        scale.Y = pVectorBasis[1]->Length();
        scale.Z = pVectorBasis[2]->Length();

        uint32 a, b, c;
        value_type x = pfScales[0], y = pfScales[1], z = pfScales[2];
        if (x < y)
        {
            if (y < z)
            {
                a = 2;
                b = 1;
                c = 0;
            }
            else
            {
                a = 1;

                if (x < z)
                {
                    b = 2;
                    c = 0;
                }
                else
                {
                    b = 0;
                    c = 2;
                }
            }
        }
        else
        {
            if (x < z)
            {
                a = 2;
                b = 0;
                c = 1;
            }
            else
            {
                a = 0;

                if (y < z)
                {
                    b = 2;
                    c = 1;
                }
                else
                {
                    b = 1;
                    c = 2;
                }
            }
        }

        if (pfScales[a] < EPSILON)
        {
            *(pVectorBasis[a]) = pCanonicalBasis[a];
        }

        *pVectorBasis[a] = Vector3::Normalize(*pVectorBasis[a]);

        if (pfScales[b] < EPSILON)
        {
            uint32 cc;
            value_type fAbsX, fAbsY, fAbsZ;

            fAbsX = (value_type)Math::Abs(pVectorBasis[a]->X);
            fAbsY = (value_type)Math::Abs(pVectorBasis[a]->Y);
            fAbsZ = (value_type)Math::Abs(pVectorBasis[a]->Z);

            if (fAbsX < fAbsY)
            {
                if (fAbsY < fAbsZ)
                {
                    cc = 0;
                }
                else
                {
                    if (fAbsX < fAbsZ)
                    {
                        cc = 0;
                    }
                    else
                    {
                        cc = 2;
                    }
                }
            }
            else
            {
                if (fAbsX < fAbsZ)
                {
                    cc = 1;
                }
                else
                {
                    if (fAbsY < fAbsZ)
                    {
                        cc = 1;
                    }
                    else
                    {
                        cc = 2;
                    }
                }
            }
            *pVectorBasis[b] = Vector3::Cross(*pVectorBasis[a], *(pCanonicalBasis + cc));
        }

        *pVectorBasis[b] = Vector3::Normalize(*pVectorBasis[b]);

        if (pfScales[c] < EPSILON)
        {
            *pVectorBasis[c] = Vector3::Cross(*pVectorBasis[a], *pVectorBasis[b]);
        }

        *pVectorBasis[c] = Vector3::Normalize(*pVectorBasis[c]);

        det = matTemp.GetDeterminant();

        // use Kramer's rule to check for handedness of coordinate system
        if (det < 0.0f)
        {
            // switch coordinate system by negating the scale and inverting the basis vector on the x-axis
            pfScales[a] = -pfScales[a];
            *pVectorBasis[a] = -(*pVectorBasis[a]);

            det = -det;
        }

        det -= 1.0f;
        det *= det;

        if ((EPSILON < det))
        {
            // Non-SRT matrix encountered
            rotation = Quaternion::Identity;
            result = false;
        }
        else
        {
            // generate the quaternion from the matrix
            rotation = Quaternion::CreateFromRotationMatrix(matTemp);
        }
		return result;
	}

	/// Transforms the given matrix by applying the given Quaternion rotation.
	/// @param value The source matrix to transform.
	/// @param rotation The rotation to apply.
	/// @return The transformed matrix.
	static Matrix4x4 Transform(const Matrix4x4& value, const Quaternion& rotation)
	{
		// Compute rotation matrix.
		value_type x2 = rotation.X + rotation.X;
		value_type y2 = rotation.Y + rotation.Y;
		value_type z2 = rotation.Z + rotation.Z;

		value_type wx2 = rotation.W * x2;
		value_type wy2 = rotation.W * y2;
		value_type wz2 = rotation.W * z2;
		value_type xx2 = rotation.X * x2;
		value_type xy2 = rotation.X * y2;
		value_type xz2 = rotation.X * z2;
		value_type yy2 = rotation.Y * y2;
		value_type yz2 = rotation.Y * z2;
		value_type zz2 = rotation.Z * z2;

		value_type q11 = 1.0f - yy2 - zz2;
		value_type q21 = xy2 - wz2;
		value_type q31 = xz2 + wy2;

		value_type q12 = xy2 + wz2;
		value_type q22 = 1.0f - xx2 - zz2;
		value_type q32 = yz2 - wx2;

		value_type q13 = xz2 - wy2;
		value_type q23 = yz2 + wx2;
		value_type q33 = 1.0f - xx2 - yy2;

		Matrix4x4 result;

		// First row
		result.M11 = value.M11 * q11 + value.M12 * q21 + value.M13 * q31;
		result.M12 = value.M11 * q12 + value.M12 * q22 + value.M13 * q32;
		result.M13 = value.M11 * q13 + value.M12 * q23 + value.M13 * q33;
		result.M14 = value.M14;

		// Second row
		result.M21 = value.M21 * q11 + value.M22 * q21 + value.M23 * q31;
		result.M22 = value.M21 * q12 + value.M22 * q22 + value.M23 * q32;
		result.M23 = value.M21 * q13 + value.M22 * q23 + value.M23 * q33;
		result.M24 = value.M24;

		// Third row
		result.M31 = value.M31 * q11 + value.M32 * q21 + value.M33 * q31;
		result.M32 = value.M31 * q12 + value.M32 * q22 + value.M33 * q32;
		result.M33 = value.M31 * q13 + value.M32 * q23 + value.M33 * q33;
		result.M34 = value.M34;

		// Fourth row
		result.M41 = value.M41 * q11 + value.M42 * q21 + value.M43 * q31;
		result.M42 = value.M41 * q12 + value.M42 * q22 + value.M43 * q32;
		result.M43 = value.M41 * q13 + value.M42 * q23 + value.M43 * q33;
		result.M44 = value.M44;

		return result;
	}


	/// Transposes the rows and columns of a matrix.
	/// @param matrix The source matrix.
	/// @return The transposed matrix.
	static Matrix4x4 Transpose(const Matrix4x4& matrix)
	{
		Matrix4x4 result;
		result.M11 = matrix.M11;
		result.M12 = matrix.M21;
		result.M13 = matrix.M31;
		result.M14 = matrix.M41;
		result.M21 = matrix.M12;
		result.M22 = matrix.M22;
		result.M23 = matrix.M32;
		result.M24 = matrix.M42;
		result.M31 = matrix.M13;
		result.M32 = matrix.M23;
		result.M33 = matrix.M33;
		result.M34 = matrix.M43;
		result.M41 = matrix.M14;
		result.M42 = matrix.M24;
		result.M43 = matrix.M34;
		result.M44 = matrix.M44;
		return result;
	}


	/// Linearly interpolates between the corresponding values of two matrices.
	/// @param matrix1 The first source matrix.
	/// @param matrix2 The second source matrix.
	/// @param amount The relative weight of the second source matrix.
	/// @return The interpolated matrix.
	static Matrix4x4 Lerp(const Matrix4x4& matrix1, const Matrix4x4& matrix2, value_type amount)
	{
		Matrix4x4 result;

		// First row
		result.M11 = matrix1.M11 + (matrix2.M11 - matrix1.M11) * amount;
		result.M12 = matrix1.M12 + (matrix2.M12 - matrix1.M12) * amount;
		result.M13 = matrix1.M13 + (matrix2.M13 - matrix1.M13) * amount;
		result.M14 = matrix1.M14 + (matrix2.M14 - matrix1.M14) * amount;

		// Second row
		result.M21 = matrix1.M21 + (matrix2.M21 - matrix1.M21) * amount;
		result.M22 = matrix1.M22 + (matrix2.M22 - matrix1.M22) * amount;
		result.M23 = matrix1.M23 + (matrix2.M23 - matrix1.M23) * amount;
		result.M24 = matrix1.M24 + (matrix2.M24 - matrix1.M24) * amount;

		// Third row
		result.M31 = matrix1.M31 + (matrix2.M31 - matrix1.M31) * amount;
		result.M32 = matrix1.M32 + (matrix2.M32 - matrix1.M32) * amount;
		result.M33 = matrix1.M33 + (matrix2.M33 - matrix1.M33) * amount;
		result.M34 = matrix1.M34 + (matrix2.M34 - matrix1.M34) * amount;

		// Fourth row
		result.M41 = matrix1.M41 + (matrix2.M41 - matrix1.M41) * amount;
		result.M42 = matrix1.M42 + (matrix2.M42 - matrix1.M42) * amount;
		result.M43 = matrix1.M43 + (matrix2.M43 - matrix1.M43) * amount;
		result.M44 = matrix1.M44 + (matrix2.M44 - matrix1.M44) * amount;

		return result;
	}


	/// Returns a new matrix with the negated elements of the given matrix.
	/// @param value The source matrix.
	/// @return The negated matrix.
	static Matrix4x4 Negate(const Matrix4x4& value)
	{
		Matrix4x4 result;
		result.M11 = -value.M11;
		result.M12 = -value.M12;
		result.M13 = -value.M13;
		result.M14 = -value.M14;
		result.M21 = -value.M21;
		result.M22 = -value.M22;
		result.M23 = -value.M23;
		result.M24 = -value.M24;
		result.M31 = -value.M31;
		result.M32 = -value.M32;
		result.M33 = -value.M33;
		result.M34 = -value.M34;
		result.M41 = -value.M41;
		result.M42 = -value.M42;
		result.M43 = -value.M43;
		result.M44 = -value.M44;
		return result;
	}


	/// Adds two matrices together.
	/// @param value1 The first source matrix.
	/// @param value2 The second source matrix.
	/// @return The resulting matrix.
	static Matrix4x4 Add(const Matrix4x4& value1, const Matrix4x4& value2)
	{
		Matrix4x4 result;
		result.M11 = value1.M11 + value2.M11;
		result.M12 = value1.M12 + value2.M12;
		result.M13 = value1.M13 + value2.M13;
		result.M14 = value1.M14 + value2.M14;
		result.M21 = value1.M21 + value2.M21;
		result.M22 = value1.M22 + value2.M22;
		result.M23 = value1.M23 + value2.M23;
		result.M24 = value1.M24 + value2.M24;
		result.M31 = value1.M31 + value2.M31;
		result.M32 = value1.M32 + value2.M32;
		result.M33 = value1.M33 + value2.M33;
		result.M34 = value1.M34 + value2.M34;
		result.M41 = value1.M41 + value2.M41;
		result.M42 = value1.M42 + value2.M42;
		result.M43 = value1.M43 + value2.M43;
		result.M44 = value1.M44 + value2.M44;
		return result;
	}


	/// Subtracts the second matrix from the first.
	/// @param value1 The first source matrix.
	/// @param value2 The second source matrix.
	/// @return The result of the subtraction.
	static Matrix4x4 Subtract(const Matrix4x4& value1, const Matrix4x4& value2)
	{
		Matrix4x4 result;

		result.M11 = value1.M11 - value2.M11;
		result.M12 = value1.M12 - value2.M12;
		result.M13 = value1.M13 - value2.M13;
		result.M14 = value1.M14 - value2.M14;
		result.M21 = value1.M21 - value2.M21;
		result.M22 = value1.M22 - value2.M22;
		result.M23 = value1.M23 - value2.M23;
		result.M24 = value1.M24 - value2.M24;
		result.M31 = value1.M31 - value2.M31;
		result.M32 = value1.M32 - value2.M32;
		result.M33 = value1.M33 - value2.M33;
		result.M34 = value1.M34 - value2.M34;
		result.M41 = value1.M41 - value2.M41;
		result.M42 = value1.M42 - value2.M42;
		result.M43 = value1.M43 - value2.M43;
		result.M44 = value1.M44 - value2.M44;

		return result;
	}


	/// Multiplies a matrix by another matrix.
	/// @param value1 The first source matrix.
	/// @param value2 The second source matrix.
	/// @return The result of the multiplication.
	static Matrix4x4 Multiply(const Matrix4x4& value1, const Matrix4x4& value2)
	{
		Matrix4x4 result;

		// First row
		result.M11 = value1.M11 * value2.M11 + value1.M12 * value2.M21 + value1.M13 * value2.M31 + value1.M14 * value2.M41;
		result.M12 = value1.M11 * value2.M12 + value1.M12 * value2.M22 + value1.M13 * value2.M32 + value1.M14 * value2.M42;
		result.M13 = value1.M11 * value2.M13 + value1.M12 * value2.M23 + value1.M13 * value2.M33 + value1.M14 * value2.M43;
		result.M14 = value1.M11 * value2.M14 + value1.M12 * value2.M24 + value1.M13 * value2.M34 + value1.M14 * value2.M44;

		// Second row
		result.M21 = value1.M21 * value2.M11 + value1.M22 * value2.M21 + value1.M23 * value2.M31 + value1.M24 * value2.M41;
		result.M22 = value1.M21 * value2.M12 + value1.M22 * value2.M22 + value1.M23 * value2.M32 + value1.M24 * value2.M42;
		result.M23 = value1.M21 * value2.M13 + value1.M22 * value2.M23 + value1.M23 * value2.M33 + value1.M24 * value2.M43;
		result.M24 = value1.M21 * value2.M14 + value1.M22 * value2.M24 + value1.M23 * value2.M34 + value1.M24 * value2.M44;

		// Third row
		result.M31 = value1.M31 * value2.M11 + value1.M32 * value2.M21 + value1.M33 * value2.M31 + value1.M34 * value2.M41;
		result.M32 = value1.M31 * value2.M12 + value1.M32 * value2.M22 + value1.M33 * value2.M32 + value1.M34 * value2.M42;
		result.M33 = value1.M31 * value2.M13 + value1.M32 * value2.M23 + value1.M33 * value2.M33 + value1.M34 * value2.M43;
		result.M34 = value1.M31 * value2.M14 + value1.M32 * value2.M24 + value1.M33 * value2.M34 + value1.M34 * value2.M44;

		// Fourth row
		result.M41 = value1.M41 * value2.M11 + value1.M42 * value2.M21 + value1.M43 * value2.M31 + value1.M44 * value2.M41;
		result.M42 = value1.M41 * value2.M12 + value1.M42 * value2.M22 + value1.M43 * value2.M32 + value1.M44 * value2.M42;
		result.M43 = value1.M41 * value2.M13 + value1.M42 * value2.M23 + value1.M43 * value2.M33 + value1.M44 * value2.M43;
		result.M44 = value1.M41 * value2.M14 + value1.M42 * value2.M24 + value1.M43 * value2.M34 + value1.M44 * value2.M44;

		return result;
	}


	/// Multiplies a matrix by a scalar value.
	/// @param value1 The source matrix.
	/// @param value2 The scaling factor.
	/// @return The scaled matrix.
	static Matrix4x4 Multiply(const Matrix4x4& value1, value_type value2)
	{
		Matrix4x4 result;
		result.M11 = value1.M11 * value2;
		result.M12 = value1.M12 * value2;
		result.M13 = value1.M13 * value2;
		result.M14 = value1.M14 * value2;
		result.M21 = value1.M21 * value2;
		result.M22 = value1.M22 * value2;
		result.M23 = value1.M23 * value2;
		result.M24 = value1.M24 * value2;
		result.M31 = value1.M31 * value2;
		result.M32 = value1.M32 * value2;
		result.M33 = value1.M33 * value2;
		result.M34 = value1.M34 * value2;
		result.M41 = value1.M41 * value2;
		result.M42 = value1.M42 * value2;
		result.M43 = value1.M43 * value2;
		result.M44 = value1.M44 * value2;
		return result;
	}

public: /// operator override

	/// Returns a new matrix with the negated elements of the given matrix.
	/// @return The negated matrix.
	inline Matrix4x4 operator -()const
	{
		Matrix4x4 m;

		m.M11 = -this->M11;
		m.M12 = -this->M12;
		m.M13 = -this->M13;
		m.M14 = -this->M14;
		m.M21 = -this->M21;
		m.M22 = -this->M22;
		m.M23 = -this->M23;
		m.M24 = -this->M24;
		m.M31 = -this->M31;
		m.M32 = -this->M32;
		m.M33 = -this->M33;
		m.M34 = -this->M34;
		m.M41 = -this->M41;
		m.M42 = -this->M42;
		m.M43 = -this->M43;
		m.M44 = -this->M44;
		return m;
	}

	/// Adds two matrices together.
	/// @param right The source matrix.
	/// @return The resulting matrix.
	inline Matrix4x4 operator +(const Matrix4x4& right)const
	{
		Matrix4x4 m;

		m.M11 = this->M11 + right.M11;
		m.M12 = this->M12 + right.M12;
		m.M13 = this->M13 + right.M13;
		m.M14 = this->M14 + right.M14;
		m.M21 = this->M21 + right.M21;
		m.M22 = this->M22 + right.M22;
		m.M23 = this->M23 + right.M23;
		m.M24 = this->M24 + right.M24;
		m.M31 = this->M31 + right.M31;
		m.M32 = this->M32 + right.M32;
		m.M33 = this->M33 + right.M33;
		m.M34 = this->M34 + right.M34;
		m.M41 = this->M41 + right.M41;
		m.M42 = this->M42 + right.M42;
		m.M43 = this->M43 + right.M43;
		m.M44 = this->M44 + right.M44;

		return m;
	}

	/// Subtracts the second matrix from the first.
	/// @param right The source matrix.
	/// @return The result of the subtraction.
	inline Matrix4x4 operator -(const Matrix4x4& right)const
	{
		Matrix4x4 m;
		m.M11 = this->M11 - right.M11;
		m.M12 = this->M12 - right.M12;
		m.M13 = this->M13 - right.M13;
		m.M14 = this->M14 - right.M14;
		m.M21 = this->M21 - right.M21;
		m.M22 = this->M22 - right.M22;
		m.M23 = this->M23 - right.M23;
		m.M24 = this->M24 - right.M24;
		m.M31 = this->M31 - right.M31;
		m.M32 = this->M32 - right.M32;
		m.M33 = this->M33 - right.M33;
		m.M34 = this->M34 - right.M34;
		m.M41 = this->M41 - right.M41;
		m.M42 = this->M42 - right.M42;
		m.M43 = this->M43 - right.M43;
		m.M44 = this->M44 - right.M44;
		return m;
	}

	/// Multiplies a matrix by another matrix.
	/// @param right The source matrix.
	/// @return The result of the multiplication.
	inline Matrix4x4 operator *(const Matrix4x4& right)const
	{
		Matrix4x4 m;

		// First row
		m.M11 = this->M11 * right.M11 + this->M12 * right.M21 + this->M13 * right.M31 + this->M14 * right.M41;
		m.M12 = this->M11 * right.M12 + this->M12 * right.M22 + this->M13 * right.M32 + this->M14 * right.M42;
		m.M13 = this->M11 * right.M13 + this->M12 * right.M23 + this->M13 * right.M33 + this->M14 * right.M43;
		m.M14 = this->M11 * right.M14 + this->M12 * right.M24 + this->M13 * right.M34 + this->M14 * right.M44;

		// Second row
		m.M21 = this->M21 * right.M11 + this->M22 * right.M21 + this->M23 * right.M31 + this->M24 * right.M41;
		m.M22 = this->M21 * right.M12 + this->M22 * right.M22 + this->M23 * right.M32 + this->M24 * right.M42;
		m.M23 = this->M21 * right.M13 + this->M22 * right.M23 + this->M23 * right.M33 + this->M24 * right.M43;
		m.M24 = this->M21 * right.M14 + this->M22 * right.M24 + this->M23 * right.M34 + this->M24 * right.M44;

		// Third row
		m.M31 = this->M31 * right.M11 + this->M32 * right.M21 + this->M33 * right.M31 + this->M34 * right.M41;
		m.M32 = this->M31 * right.M12 + this->M32 * right.M22 + this->M33 * right.M32 + this->M34 * right.M42;
		m.M33 = this->M31 * right.M13 + this->M32 * right.M23 + this->M33 * right.M33 + this->M34 * right.M43;
		m.M34 = this->M31 * right.M14 + this->M32 * right.M24 + this->M33 * right.M34 + this->M34 * right.M44;

		// Fourth row
		m.M41 = this->M41 * right.M11 + this->M42 * right.M21 + this->M43 * right.M31 + this->M44 * right.M41;
		m.M42 = this->M41 * right.M12 + this->M42 * right.M22 + this->M43 * right.M32 + this->M44 * right.M42;
		m.M43 = this->M41 * right.M13 + this->M42 * right.M23 + this->M43 * right.M33 + this->M44 * right.M43;
		m.M44 = this->M41 * right.M14 + this->M42 * right.M24 + this->M43 * right.M34 + this->M44 * right.M44;

		return m;
	}

	/// Multiplies a matrix by a scalar value.
	/// @param right The scaling factor.
	/// @return The scaled matrix.
	inline Matrix4x4 operator *(value_type right)const
	{
		Matrix4x4 m;

		m.M11 = this->M11 * right;
		m.M12 = this->M12 * right;
		m.M13 = this->M13 * right;
		m.M14 = this->M14 * right;
		m.M21 = this->M21 * right;
		m.M22 = this->M22 * right;
		m.M23 = this->M23 * right;
		m.M24 = this->M24 * right;
		m.M31 = this->M31 * right;
		m.M32 = this->M32 * right;
		m.M33 = this->M33 * right;
		m.M34 = this->M34 * right;
		m.M41 = this->M41 * right;
		m.M42 = this->M42 * right;
		m.M43 = this->M43 * right;
		m.M44 = this->M44 * right;
		return m;
	}

	/// Returns a boolean indicating whether the given two matrices are equal.
	/// @param right The target matrix to compare.
	/// @return True if the given matrices are equal; False otherwise.
	inline bool operator ==(const Matrix4x4& right)const
	{
        return this->Equals(right);
	}

	/// Returns a boolean indicating whether the given two matrices are not equal.
	/// @param right The target matrix to compare.
	/// @return True if the given matrices are not equal; False if they are equal.
	inline bool operator !=(const Matrix4x4& right)const
	{
		return !((*this) == right);
	}
};
//
///// Returns a new matrix with the negated elements of the given matrix.
///// @param value The source matrix.
///// @return The negated matrix.
//static Matrix4x4 operator -(const Matrix4x4& value)
//{
//    Matrix4x4 m;
//
//    m.M11 = -value.M11;
//    m.M12 = -value.M12;
//    m.M13 = -value.M13;
//    m.M14 = -value.M14;
//    m.M21 = -value.M21;
//    m.M22 = -value.M22;
//    m.M23 = -value.M23;
//    m.M24 = -value.M24;
//    m.M31 = -value.M31;
//    m.M32 = -value.M32;
//    m.M33 = -value.M33;
//    m.M34 = -value.M34;
//    m.M41 = -value.M41;
//    m.M42 = -value.M42;
//    m.M43 = -value.M43;
//    m.M44 = -value.M44;
//    return m;
//}
//
///// Adds two matrices together.
///// @param value1 The first source matrix.
///// @param value2 The second source matrix.
///// @return The resulting matrix.
//static Matrix4x4 operator +(const Matrix4x4& value1, const Matrix4x4& value2)
//{
//    Matrix4x4 m;
//
//    m.M11 = value1.M11 + value2.M11;
//    m.M12 = value1.M12 + value2.M12;
//    m.M13 = value1.M13 + value2.M13;
//    m.M14 = value1.M14 + value2.M14;
//    m.M21 = value1.M21 + value2.M21;
//    m.M22 = value1.M22 + value2.M22;
//    m.M23 = value1.M23 + value2.M23;
//    m.M24 = value1.M24 + value2.M24;
//    m.M31 = value1.M31 + value2.M31;
//    m.M32 = value1.M32 + value2.M32;
//    m.M33 = value1.M33 + value2.M33;
//    m.M34 = value1.M34 + value2.M34;
//    m.M41 = value1.M41 + value2.M41;
//    m.M42 = value1.M42 + value2.M42;
//    m.M43 = value1.M43 + value2.M43;
//    m.M44 = value1.M44 + value2.M44;
//
//    return m;
//}
//
///// <summary>
///// Subtracts the second matrix from the first.
///// </summary>
///// @param value1 The first source matrix.
///// @param value2 The second source matrix.
///// @return The result of the subtraction.
//static Matrix4x4 operator -(const Matrix4x4& value1, const Matrix4x4& value2)
//{
//    Matrix4x4 m;
//
//    m.M11 = value1.M11 - value2.M11;
//    m.M12 = value1.M12 - value2.M12;
//    m.M13 = value1.M13 - value2.M13;
//    m.M14 = value1.M14 - value2.M14;
//    m.M21 = value1.M21 - value2.M21;
//    m.M22 = value1.M22 - value2.M22;
//    m.M23 = value1.M23 - value2.M23;
//    m.M24 = value1.M24 - value2.M24;
//    m.M31 = value1.M31 - value2.M31;
//    m.M32 = value1.M32 - value2.M32;
//    m.M33 = value1.M33 - value2.M33;
//    m.M34 = value1.M34 - value2.M34;
//    m.M41 = value1.M41 - value2.M41;
//    m.M42 = value1.M42 - value2.M42;
//    m.M43 = value1.M43 - value2.M43;
//    m.M44 = value1.M44 - value2.M44;
//
//    return m;
//}
//
///// Multiplies a matrix by another matrix.
///// @param value1 The first source matrix.
///// @param value2 The second source matrix.
///// @return The result of the multiplication.
//static Matrix4x4 operator *(const Matrix4x4& value1, const Matrix4x4& value2)
//{
//    Matrix4x4 m;
//
//    // First row
//    m.M11 = value1.M11 * value2.M11 + value1.M12 * value2.M21 + value1.M13 * value2.M31 + value1.M14 * value2.M41;
//    m.M12 = value1.M11 * value2.M12 + value1.M12 * value2.M22 + value1.M13 * value2.M32 + value1.M14 * value2.M42;
//    m.M13 = value1.M11 * value2.M13 + value1.M12 * value2.M23 + value1.M13 * value2.M33 + value1.M14 * value2.M43;
//    m.M14 = value1.M11 * value2.M14 + value1.M12 * value2.M24 + value1.M13 * value2.M34 + value1.M14 * value2.M44;
//
//    // Second row
//    m.M21 = value1.M21 * value2.M11 + value1.M22 * value2.M21 + value1.M23 * value2.M31 + value1.M24 * value2.M41;
//    m.M22 = value1.M21 * value2.M12 + value1.M22 * value2.M22 + value1.M23 * value2.M32 + value1.M24 * value2.M42;
//    m.M23 = value1.M21 * value2.M13 + value1.M22 * value2.M23 + value1.M23 * value2.M33 + value1.M24 * value2.M43;
//    m.M24 = value1.M21 * value2.M14 + value1.M22 * value2.M24 + value1.M23 * value2.M34 + value1.M24 * value2.M44;
//
//    // Third row
//    m.M31 = value1.M31 * value2.M11 + value1.M32 * value2.M21 + value1.M33 * value2.M31 + value1.M34 * value2.M41;
//    m.M32 = value1.M31 * value2.M12 + value1.M32 * value2.M22 + value1.M33 * value2.M32 + value1.M34 * value2.M42;
//    m.M33 = value1.M31 * value2.M13 + value1.M32 * value2.M23 + value1.M33 * value2.M33 + value1.M34 * value2.M43;
//    m.M34 = value1.M31 * value2.M14 + value1.M32 * value2.M24 + value1.M33 * value2.M34 + value1.M34 * value2.M44;
//
//    // Fourth row
//    m.M41 = value1.M41 * value2.M11 + value1.M42 * value2.M21 + value1.M43 * value2.M31 + value1.M44 * value2.M41;
//    m.M42 = value1.M41 * value2.M12 + value1.M42 * value2.M22 + value1.M43 * value2.M32 + value1.M44 * value2.M42;
//    m.M43 = value1.M41 * value2.M13 + value1.M42 * value2.M23 + value1.M43 * value2.M33 + value1.M44 * value2.M43;
//    m.M44 = value1.M41 * value2.M14 + value1.M42 * value2.M24 + value1.M43 * value2.M34 + value1.M44 * value2.M44;
//
//    return m;
//}
//
///// Multiplies a matrix by a scalar value.
///// @param value1 The source matrix.
///// @param value2 The scaling factor.
///// @return The scaled matrix.
//static Matrix4x4 operator *(const Matrix4x4& value1, value_type value2)
//{
//    Matrix4x4 m;
//
//    m.M11 = value1.M11 * value2;
//    m.M12 = value1.M12 * value2;
//    m.M13 = value1.M13 * value2;
//    m.M14 = value1.M14 * value2;
//    m.M21 = value1.M21 * value2;
//    m.M22 = value1.M22 * value2;
//    m.M23 = value1.M23 * value2;
//    m.M24 = value1.M24 * value2;
//    m.M31 = value1.M31 * value2;
//    m.M32 = value1.M32 * value2;
//    m.M33 = value1.M33 * value2;
//    m.M34 = value1.M34 * value2;
//    m.M41 = value1.M41 * value2;
//    m.M42 = value1.M42 * value2;
//    m.M43 = value1.M43 * value2;
//    m.M44 = value1.M44 * value2;
//    return m;
//}
//
///// Returns a boolean indicating whether the given two matrices are equal.
///// @param value1 The first matrix to compare.
///// @param value2 The second matrix to compare.
///// @return True if the given matrices are equal; False otherwise.
//static bool operator ==(const Matrix4x4& value1, const Matrix4x4& value2)
//{
//    return (value1.M11 == value2.M11 && value1.M22 == value2.M22 && value1.M33 == value2.M33 && value1.M44 == value2.M44 && // Check diagonal element first for early out.
//        value1.M12 == value2.M12 && value1.M13 == value2.M13 && value1.M14 == value2.M14 &&
//        value1.M21 == value2.M21 && value1.M23 == value2.M23 && value1.M24 == value2.M24 &&
//        value1.M31 == value2.M31 && value1.M32 == value2.M32 && value1.M34 == value2.M34 &&
//        value1.M41 == value2.M41 && value1.M42 == value2.M42 && value1.M43 == value2.M43);
//}
//
///// Returns a boolean indicating whether the given two matrices are not equal.
///// @param value1 The first matrix to compare.
///// @param value2 The second matrix to compare.
///// @return True if the given matrices are not equal; False if they are equal.
//static bool operator !=(const Matrix4x4& value1, const Matrix4x4& value2)
//{
//    return (value1.M11 != value2.M11 || value1.M12 != value2.M12 || value1.M13 != value2.M13 || value1.M14 != value2.M14 ||
//        value1.M21 != value2.M21 || value1.M22 != value2.M22 || value1.M23 != value2.M23 || value1.M24 != value2.M24 ||
//        value1.M31 != value2.M31 || value1.M32 != value2.M32 || value1.M33 != value2.M33 || value1.M34 != value2.M34 ||
//        value1.M41 != value2.M41 || value1.M42 != value2.M42 || value1.M43 != value2.M43 || value1.M44 != value2.M44);
//}

NS_FK_END

#endif // END OF FOUNDATIONKIT_MATRIX4X4_HPP
