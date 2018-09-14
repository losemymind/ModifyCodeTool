/****************************************************************************
  Copyright (c) 2018 libo All rights reserved.
  
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_MATRIX3X2_HPP
#define FOUNDATIONKIT_MATRIX3X2_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)
#include <cmath>
#include "FoundationKit/GenericPlatformMacros.hpp"
#include "FoundationKit/Foundation/StringUtils.hpp"
#include "FoundationKit/Math/Math.hpp"
#include "FoundationKit/Math/Vector2.hpp"
NS_FK_BEGIN
/// A structure encapsulating a 3x2 matrix.
struct Matrix3x2
{
    typedef float value_type;
    /// The first element of the first row
    value_type M11;
    /// The second element of the first row
    value_type M12;
    /// The first element of the second row
    value_type M21;
    /// The second element of the second row
    value_type M22;
    /// The first element of the third row
    value_type M31;
    /// The second element of the third row
    value_type M32;

    /// Returns the multiplicative identity matrix.
    static const Matrix3x2 Identity;

    /// Constructs a Matrix3x2 from the given components.
    Matrix3x2(value_type m11, value_type m12,
        value_type m21, value_type m22,
        value_type m31, value_type m32)
        : M11(m11)
        , M12(m12)
        , M21(m21)
        , M22(m22)
        , M31(m31)
        , M32(m32)
    {

    }

    /// Constructs a Matrix3x2 from the given components.
    Matrix3x2() : Matrix3x2(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f) {}

    /// Returns a boolean indicating whether the matrix is equal to the other given matrix.
    /// @param other The other matrix to test equality against.
    /// @return True if this matrix is equal to other; False otherwise.
    bool Equals(const Matrix3x2& other)const
    {
        return (M11 == other.M11 && M22 == other.M22 && // Check diagonal element first for early out.
            M12 == other.M12 &&
            M21 == other.M21 &&
            M31 == other.M31 && M32 == other.M32);
    }


    /// Returns a String representing this matrix instance.
    /// @return The string representation.
    std::string ToString()
    {
        return StringUtils::Format("[[M11:%f, M12:%f],[M21:%f, M22:%f],[M31:%f, M32:%f]]",
            M11, M12,
            M21, M22,
            M31, M32);
    }

    /// Returns the hash code for this instance.
    /// @return The hash code.
    size_t GetHashCode()
    {
        auto hasher = std::hash<float>{};
        return (hasher(M11) + hasher(M12) + hasher(M21) + hasher(M22) + hasher(M31) + hasher(M32));
    }

   /// Returns whether the matrix is the identity matrix.
    bool IsIdentity()
    {
        return M11 == 1.0f && M22 == 1.0f && // Check diagonal element first for early out.
            M12 == 0.0f &&
            M21 == 0.0f &&
            M31 == 0.0f && 
            M32 == 0.0f;
    }

    /// Gets the translation component of this matrix.
    Vector2 GetTranslation()
    {
        return Vector2(M31, M32);
    }

    /// Sets the translation component of this matrix.
    /// @param value The translation position.
    void SetTranslation(const Vector2& value)
    {
        M31 = value.X;
        M32 = value.Y;
    }

    /// Calculates the determinant for this matrix. 
    /// The determinant is calculated by expanding the matrix with a third column whose values are (0,0,1).
    /// @return The determinant.
    value_type GetDeterminant()
    {
        // There isn't actually any such thing as a determinant for a non-square matrix,
        // but this 3x2 type is really just an optimization of a 3x3 where we happen to
        // know the rightmost column is always (0, 0, 1). So we expand to 3x3 format:
        //
        //  [ M11, M12, 0 ]
        //  [ M21, M22, 0 ]
        //  [ M31, M32, 1 ]
        //
        // Sum the diagonal products:
        //  (M11 * M22 * 1) + (M12 * 0 * M31) + (0 * M21 * M32)
        //
        // Subtract the opposite diagonal products:
        //  (M31 * M22 * 0) + (M32 * 0 * M11) + (1 * M21 * M12)
        //
        // Collapse out the constants and oh look, this is just a 2x2 determinant!

        return (M11 * M22) - (M21 * M12);
    }

    value_type* Ptr()
    {
        return reinterpret_cast<value_type*>(this);
    }

public: /// static functions

    /// Creates a translation matrix from the given vector.
    /// @param position The translation position.
    /// @return A translation matrix.
    static Matrix3x2 CreateTranslation(const Vector2& position)
    {
        Matrix3x2 result;

        result.M11 = 1.0f;
        result.M12 = 0.0f;
        result.M21 = 0.0f;
        result.M22 = 1.0f;

        result.M31 = position.X;
        result.M32 = position.Y;

        return result;
    }

    /// Creates a translation matrix from the given X and Y components.
    /// @param xPosition The X position.
    /// @param yPosition The Y position.
    /// @return A translation matrix.
    static Matrix3x2 CreateTranslation(value_type xPosition, value_type yPosition)
    {
        Matrix3x2 result;

        result.M11 = 1.0f;
        result.M12 = 0.0f;
        result.M21 = 0.0f;
        result.M22 = 1.0f;

        result.M31 = xPosition;
        result.M32 = yPosition;

        return result;
    }

    /// Creates a scale matrix from the given X and Y components.
    /// @param xScale Value to scale by on the X-axis.
    /// @param yScale Value to scale by on the Y-axis.
    /// @return A scaling matrix.
    static Matrix3x2 CreateScale(value_type xScale, value_type yScale)
    {
        Matrix3x2 result;

        result.M11 = xScale;
        result.M12 = 0.0f;
        result.M21 = 0.0f;
        result.M22 = yScale;
        result.M31 = 0.0f;
        result.M32 = 0.0f;

        return result;
    }

    /// Creates a scale matrix that is offset by a given center point.
    /// @param xScale Value to scale by on the X-axis.
    /// @param yScale Value to scale by on the Y-axis.
    /// @param centerPoint The center point.
    /// @return A scaling matrix.
    static Matrix3x2 CreateScale(value_type xScale, value_type yScale, const Vector2& centerPoint)
    {
        Matrix3x2 result;

        value_type tx = centerPoint.X * (1 - xScale);
        value_type ty = centerPoint.Y * (1 - yScale);

        result.M11 = xScale;
        result.M12 = 0.0f;
        result.M21 = 0.0f;
        result.M22 = yScale;
        result.M31 = tx;
        result.M32 = ty;

        return result;
    }

    /// Creates a scale matrix from the given vector scale.
    /// @param scales The scale to use.
    /// @return A scaling matrix.
    static Matrix3x2 CreateScale(const Vector2& scales)
    {
        Matrix3x2 result;

        result.M11 = scales.X;
        result.M12 = 0.0f;
        result.M21 = 0.0f;
        result.M22 = scales.Y;
        result.M31 = 0.0f;
        result.M32 = 0.0f;

        return result;
    }
    /// Creates a scale matrix from the given vector scale with an offset from the given center point.
    /// @param scales The scale to use.
    /// @param centerPoint The center offset.
    /// @return A scaling matrix.
    static Matrix3x2 CreateScale(const Vector2& scales, const Vector2& centerPoint)
    {
        Matrix3x2 result;

        value_type tx = centerPoint.X * (1 - scales.X);
        value_type ty = centerPoint.Y * (1 - scales.Y);

        result.M11 = scales.X;
        result.M12 = 0.0f;
        result.M21 = 0.0f;
        result.M22 = scales.Y;
        result.M31 = tx;
        result.M32 = ty;

        return result;
    }

    /// Creates a scale matrix that scales uniformly with the given scale.
    /// @param scale The uniform scale to use.
    /// @return A scaling matrix.
    static Matrix3x2 CreateScale(value_type scale)
    {
        Matrix3x2 result;

        result.M11 = scale;
        result.M12 = 0.0f;
        result.M21 = 0.0f;
        result.M22 = scale;
        result.M31 = 0.0f;
        result.M32 = 0.0f;

        return result;
    }

    /// Creates a scale matrix that scales uniformly with the given scale with an offset from the given center.
    /// @param scale The uniform scale to use.
    /// @param centerPoint The center offset.
    /// @return A scaling matrix.
    static Matrix3x2 CreateScale(value_type scale, const Vector2& centerPoint)
    {
        Matrix3x2 result;

        value_type tx = centerPoint.X * (1 - scale);
        value_type ty = centerPoint.Y * (1 - scale);

        result.M11 = scale;
        result.M12 = 0.0f;
        result.M21 = 0.0f;
        result.M22 = scale;
        result.M31 = tx;
        result.M32 = ty;

        return result;
    }

    /// Creates a skew matrix from the given angles in radians.
    /// @param radiansX The X angle, in radians.
    /// @param radiansY The Y angle, in radians.
    /// @return A skew matrix.
    static Matrix3x2 CreateSkew(value_type radiansX, value_type radiansY)
    {
        Matrix3x2 result;

        value_type xTan = (value_type)Math::Tan(radiansX);
        value_type yTan = (value_type)Math::Tan(radiansY);

        result.M11 = 1.0f;
        result.M12 = yTan;
        result.M21 = xTan;
        result.M22 = 1.0f;
        result.M31 = 0.0f;
        result.M32 = 0.0f;

        return result;
    }

    /// Creates a skew matrix from the given angles in radians and a center point.
    /// @param radiansX The X angle, in radians.
    /// @param radiansY The Y angle, in radians.
    /// @param centerPoint The center point.
    /// @return A skew matrix.
    static Matrix3x2 CreateSkew(value_type radiansX, value_type radiansY, const Vector2& centerPoint)
    {
        Matrix3x2 result;

        value_type xTan = (value_type)Math::Tan(radiansX);
        value_type yTan = (value_type)Math::Tan(radiansY);

        value_type tx = -centerPoint.Y * xTan;
        value_type ty = -centerPoint.X * yTan;

        result.M11 = 1.0f;
        result.M12 = yTan;
        result.M21 = xTan;
        result.M22 = 1.0f;
        result.M31 = tx;
        result.M32 = ty;

        return result;
    }

    /// Creates a rotation matrix using the given rotation in radians.
    /// @param radians The amount of rotation, in radians.
    /// @return A rotation matrix.
    static Matrix3x2 CreateRotation(value_type radians)
    {
        Matrix3x2 result;

        radians =static_cast<value_type>(/*std*/::remainder(radians, Math::PI * 2));

        value_type c, s;

        const value_type epsilon = 0.001f * (value_type)Math::PI / 180.0f;     // 0.1% of a degree

        if (radians > -epsilon && radians < epsilon)
        {
            // Exact case for zero rotation.
            c = 1;
            s = 0;
        }
        else if (radians > Math::PI / 2 - epsilon && radians < Math::PI / 2 + epsilon)
        {
            // Exact case for 90 degree rotation.
            c = 0;
            s = 1;
        }
        else if (radians < -Math::PI + epsilon || radians > Math::PI - epsilon)
        {
            // Exact case for 180 degree rotation.
            c = -1;
            s = 0;
        }
        else if (radians > -Math::PI / 2 - epsilon && radians < -Math::PI / 2 + epsilon)
        {
            // Exact case for 270 degree rotation.
            c = 0;
            s = -1;
        }
        else
        {
            // Arbitrary rotation.
            c = (value_type)Math::Cos(radians);
            s = (value_type)Math::Sin(radians);
        }

        // [  c  s ]
        // [ -s  c ]
        // [  0  0 ]
        result.M11 = c;
        result.M12 = s;
        result.M21 = -s;
        result.M22 = c;
        result.M31 = 0.0f;
        result.M32 = 0.0f;

        return result;
    }

    /// Creates a rotation matrix using the given rotation in radians and a center point.
    /// @param radians The amount of rotation, in radians.
    /// @param centerPoint The center point.
    /// @return A rotation matrix.
    static Matrix3x2 CreateRotation(value_type radians, const Vector2& centerPoint)
    {
        Matrix3x2 result;

        radians = static_cast<value_type>(/*std*/::remainder(radians, Math::PI * 2));

        value_type c, s;

        const value_type epsilon = 0.001f * (value_type)Math::PI / 180.0f;     // 0.1% of a degree

        if (radians > -epsilon && radians < epsilon)
        {
            // Exact case for zero rotation.
            c = 1;
            s = 0;
        }
        else if (radians > Math::PI / 2 - epsilon && radians < Math::PI / 2 + epsilon)
        {
            // Exact case for 90 degree rotation.
            c = 0;
            s = 1;
        }
        else if (radians < -Math::PI + epsilon || radians > Math::PI - epsilon)
        {
            // Exact case for 180 degree rotation.
            c = -1;
            s = 0;
        }
        else if (radians > -Math::PI / 2 - epsilon && radians < -Math::PI / 2 + epsilon)
        {
            // Exact case for 270 degree rotation.
            c = 0;
            s = -1;
        }
        else
        {
            // Arbitrary rotation.
            c = (value_type)Math::Cos(radians);
            s = (value_type)Math::Sin(radians);
        }

        value_type x = centerPoint.X * (1 - c) + centerPoint.Y * s;
        value_type y = centerPoint.Y * (1 - c) - centerPoint.X * s;

        // [  c  s ]
        // [ -s  c ]
        // [  x  y ]
        result.M11 = c;
        result.M12 = s;
        result.M21 = -s;
        result.M22 = c;
        result.M31 = x;
        result.M32 = y;

        return result;
    }

    /// Attempts to invert the given matrix. If the operation succeeds, the inverted matrix is stored in the result parameter.
    /// @param matrix The source matrix.
    /// @param result The output matrix.
    /// @return True if the operation succeeded, False otherwise.
    static bool Invert(const Matrix3x2& matrix, Matrix3x2& result)
    {
        value_type det = (matrix.M11 * matrix.M22) - (matrix.M21 * matrix.M12);

        if (Math::Abs(det) < Math::Epsilon)
        {
            result = Matrix3x2();
            return false;
        }

        value_type invDet = 1.0f / det;
        result.M11 = matrix.M22 * invDet;
        result.M12 = -matrix.M12 * invDet;
        result.M21 = -matrix.M21 * invDet;
        result.M22 = matrix.M11 * invDet;
        result.M31 = (matrix.M21 * matrix.M32 - matrix.M31 * matrix.M22) * invDet;
        result.M32 = (matrix.M31 * matrix.M12 - matrix.M11 * matrix.M32) * invDet;
        return true;
    }

    /// Linearly interpolates from matrix1 to matrix2, based on the third parameter.
    /// @param matrix1 The first source matrix.
    /// @param matrix2 The second source matrix.
    /// @param amount The relative weighting of matrix2.
    /// @return The interpolated matrix.
    static Matrix3x2 Lerp(const Matrix3x2& matrix1, const Matrix3x2& matrix2, value_type amount)
    {
        Matrix3x2 result;

        // First row
        result.M11 = matrix1.M11 + (matrix2.M11 - matrix1.M11) * amount;
        result.M12 = matrix1.M12 + (matrix2.M12 - matrix1.M12) * amount;

        // Second row
        result.M21 = matrix1.M21 + (matrix2.M21 - matrix1.M21) * amount;
        result.M22 = matrix1.M22 + (matrix2.M22 - matrix1.M22) * amount;

        // Third row
        result.M31 = matrix1.M31 + (matrix2.M31 - matrix1.M31) * amount;
        result.M32 = matrix1.M32 + (matrix2.M32 - matrix1.M32) * amount;

        return result;
    }

    /// Negates the given matrix by multiplying all values by -1.
    /// @param value The source matrix.
    /// @return The negated matrix.
    static Matrix3x2 Negate(const Matrix3x2& value)
    {
        Matrix3x2 result;

        result.M11 = -value.M11;
        result.M12 = -value.M12;
        result.M21 = -value.M21;
        result.M22 = -value.M22;
        result.M31 = -value.M31;
        result.M32 = -value.M32;

        return result;
    }

    /// Adds each matrix element in value1 with its corresponding element in value2.
    /// @param value1 The first source matrix.
    /// @param value2 The second source matrix.
    /// @return The matrix containing the summed values.
    static Matrix3x2 Add(const Matrix3x2& value1, const Matrix3x2& value2)
    {
        Matrix3x2 result;

        result.M11 = value1.M11 + value2.M11;
        result.M12 = value1.M12 + value2.M12;
        result.M21 = value1.M21 + value2.M21;
        result.M22 = value1.M22 + value2.M22;
        result.M31 = value1.M31 + value2.M31;
        result.M32 = value1.M32 + value2.M32;

        return result;
    }

    /// Subtracts each matrix element in value2 from its corresponding element in value1.
    /// @param value1 The first source matrix.
    /// @param value2 The second source matrix.
    /// @return The matrix containing the resulting values.
    static Matrix3x2 Subtract(const Matrix3x2& value1, const Matrix3x2& value2)
    {
        Matrix3x2 result;

        result.M11 = value1.M11 - value2.M11;
        result.M12 = value1.M12 - value2.M12;
        result.M21 = value1.M21 - value2.M21;
        result.M22 = value1.M22 - value2.M22;
        result.M31 = value1.M31 - value2.M31;
        result.M32 = value1.M32 - value2.M32;

        return result;
    }

    /// Multiplies two matrices together and returns the resulting matrix.
    /// @param value1 The first source matrix.
    /// @param value2 The second source matrix.
    /// @return The product matrix.
    static Matrix3x2 Multiply(const Matrix3x2& value1, const Matrix3x2& value2)
    {
        Matrix3x2 result;

        // First row
        result.M11 = value1.M11 * value2.M11 + value1.M12 * value2.M21;
        result.M12 = value1.M11 * value2.M12 + value1.M12 * value2.M22;

        // Second row
        result.M21 = value1.M21 * value2.M11 + value1.M22 * value2.M21;
        result.M22 = value1.M21 * value2.M12 + value1.M22 * value2.M22;

        // Third row
        result.M31 = value1.M31 * value2.M11 + value1.M32 * value2.M21 + value2.M31;
        result.M32 = value1.M31 * value2.M12 + value1.M32 * value2.M22 + value2.M32;

        return result;
    }

    /// Scales all elements in a matrix by the given scalar factor.
    /// @param value1 The source matrix.
    /// @param value2 The scaling value to use.
    /// @return The resulting matrix.
    static Matrix3x2 Multiply(const Matrix3x2& value1, value_type value2)
    {
        Matrix3x2 result;

        result.M11 = value1.M11 * value2;
        result.M12 = value1.M12 * value2;
        result.M21 = value1.M21 * value2;
        result.M22 = value1.M22 * value2;
        result.M31 = value1.M31 * value2;
        result.M32 = value1.M32 * value2;

        return result;
    }

public: /// operator override

    /// Negates the given matrix by multiplying all values by -1.
    /// @return The negated matrix.
    inline Matrix3x2 operator -()const
    {
        Matrix3x2 m;
        m.M11 = -this->M11;
        m.M12 = -this->M12;
        m.M21 = -this->M21;
        m.M22 = -this->M22;
        m.M31 = -this->M31;
        m.M32 = -this->M32;
        return m;
    }

    /// Adds each matrix element in this with its corresponding element in right.
    /// @param right The source matrix.
    /// @return The matrix containing the summed values.
    inline Matrix3x2 operator +(const Matrix3x2& right)const
    {
        Matrix3x2 m;
        m.M11 = this->M11 + right.M11;
        m.M12 = this->M12 + right.M12;
        m.M21 = this->M21 + right.M21;
        m.M22 = this->M22 + right.M22;
        m.M31 = this->M31 + right.M31;
        m.M32 = this->M32 + right.M32;
        return m;
    }

    /// Subtracts each matrix element in right from its corresponding element in this.
    /// @param right The source matrix.
    /// @return The matrix containing the resulting values.
    inline Matrix3x2 operator -(const Matrix3x2& right)const
    {
        Matrix3x2 m;
        m.M11 = this->M11 - right.M11;
        m.M12 = this->M12 - right.M12;
        m.M21 = this->M21 - right.M21;
        m.M22 = this->M22 - right.M22;
        m.M31 = this->M31 - right.M31;
        m.M32 = this->M32 - right.M32;
        return m;
    }

    /// Multiplies two matrices together and returns the resulting matrix.
    /// @param right The source matrix.
    /// @return The product matrix.
    inline Matrix3x2 operator *(const Matrix3x2& right)const
    {
        Matrix3x2 m;

        // First row
        m.M11 = this->M11 * right.M11 + this->M12 * right.M21;
        m.M12 = this->M11 * right.M12 + this->M12 * right.M22;

        // Second row
        m.M21 = this->M21 * right.M11 + this->M22 * right.M21;
        m.M22 = this->M21 * right.M12 + this->M22 * right.M22;

        // Third row
        m.M31 = this->M31 * right.M11 + this->M32 * right.M21 + right.M31;
        m.M32 = this->M31 * right.M12 + this->M32 * right.M22 + right.M32;

        return m;
    }

    /// Scales all elements in a matrix by the given scalar factor.
    /// @param right The scaling value to use.
    /// @return The resulting matrix.
    inline Matrix3x2 operator *(value_type right)const
    {
        Matrix3x2 m;

        m.M11 = this->M11 * right;
        m.M12 = this->M12 * right;
        m.M21 = this->M21 * right;
        m.M22 = this->M22 * right;
        m.M31 = this->M31 * right;
        m.M32 = this->M32 * right;

        return m;
    }

    /// Returns a boolean indicating whether the given matrices are equal.
    /// @param right The source matrix.
    /// @return True if the matrices are equal; False otherwise.
    inline bool operator ==(const Matrix3x2& right)const
    {
        return this->Equals(right);
    }

    /// Returns a boolean indicating whether the given matrices are not equal.
    /// @param right The source matrix.
    /// @return True if the matrices are not equal; False if they are equal.
    inline bool operator !=(const Matrix3x2& right)const
    {
        return !((*this) == right);
    }
};
NS_FK_END

#endif // END OF FOUNDATIONKIT_MATRIX3X2_HPP