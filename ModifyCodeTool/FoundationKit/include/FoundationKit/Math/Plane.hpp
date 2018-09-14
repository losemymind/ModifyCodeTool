/****************************************************************************
  Copyright (c) 2018 libo All rights reserved.
  
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_PLANE_HPP
#define FOUNDATIONKIT_PLANE_HPP


#include "FoundationKit/Foundation/StringUtils.hpp"
#include "FoundationKit/Math/Vector3.hpp"
#include "FoundationKit/Math/Vector4.hpp"
NS_FK_BEGIN
struct Matrix4x4;
struct Quaternion;

/// A structure encapsulating a 3D Plane
struct Plane 
{
    typedef float value_type;
    /// The normal vector of the Plane.
    Vector3 Normal;
    /// The distance of the Plane along its normal from the origin.

    value_type D;

    /// Constructs a Plane from the X, Y, and Z components of its normal, and its distance from the origin on that normal.
    /// @param x The X-component of the normal.
    /// @param y The Y-component of the normal.
    /// @param z The Z-component of the normal.
    /// @param d The distance of the Plane along its normal from the origin.
    Plane(value_type x, value_type y, value_type z, value_type d)
        :Plane(Vector3(x, y, z), d)
    {

    }


    /// Constructs a Plane from the given normal and distance along the normal from the origin.
    /// @param normal The Plane's normal vector.
    /// @param  d The Plane's distance from the origin along its normal vector.
    Plane(const Vector3& normal, value_type d)
    {
        this->Normal = normal;
        this->D = d;
    }

    /// Constructs a Plane from the given Vector4.
    /// @param value A vector whose first 3 elements describe the normal vector, 
    /// and whose W component defines the distance along that normal from the origin.
    Plane(const Vector4& value)
    {
        Normal = Vector3(value.X, value.Y, value.Z);
        D = value.W;
    }

    /// Returns a boolean indicating whether the given Plane is equal to this Plane instance.
    /// @param other The Plane to compare this instance to.
    /// @return True if the other Plane is equal to this instance; False otherwise.
    bool Equals(Plane other)const
    {
        return (this->Normal.Equals(other.Normal) && this->D == other.D);
    }

    /// Returns a String representing this Plane instance.
    /// @return The string representation.
    std::string ToString()const
    {
        return StringUtils::Format("[Normal:%s D:%f]", Normal.ToString().c_str(), D);
    }

    /// Returns the hash code for this instance.
    /// @return The hash code.
    size_t GetHashCode()const
    {
        return Normal.GetHashCode() + std::hash<value_type>{}(D);
    }

public: /// static functions

    /// Creates a Plane that contains the three given points.
    /// @param point1 The first point defining the Plane.
    /// @param point2 The second point defining the Plane.
    /// @param point3 The third point defining the Plane.
    /// @return The Plane containing the three points.
    static Plane CreateFromVertices(const Vector3& point1, const Vector3& point2, const Vector3& point3)
    {
        Vector3 a = point2 - point1;
        Vector3 b = point3 - point1;

        // N = Cross(a, b)
        Vector3 n = Vector3::Cross(a, b);
        Vector3 normal = Vector3::Normalize(n);

        // D = - Dot(N, point1)
        value_type d = -Vector3::Dot(normal, point1);

        return Plane(normal, d);
    }
    
    /// Creates a new Plane whose normal vector is the source Plane's normal vector normalized.
    /// @param value The source Plane.
    /// @return The normalized Plane.
    static Plane Normalize(const Plane& value)
    {
        //const value_type FLT_EPSILON = 1.192092896e-07f; // smallest such that 1.0+FLT_EPSILON != 1.0
        value_type normalLengthSquared = value.Normal.LengthSquared();
        if (Math::Abs(normalLengthSquared - 1.0f) < FLT_EPSILON)
        {
            // It already normalized, so we don't need to farther process.
            return value;
        }
        value_type normalLength = (value_type)Math::Sqrt(normalLengthSquared);
        return Plane(
            value.Normal / normalLength,
            value.D / normalLength);
    }

    /// Transforms a normalized Plane by a Matrix.
    /// @param plane The normalized Plane to transform. 
    /// This Plane must already be normalized, so that its Normal vector is of unit length, before this method is called.
    /// @param matrix The transformation matrix to apply to the Plane.
    /// @return The transformed Plane.
    static Plane Transform(const Plane& plane, const Matrix4x4& matrix);


    /// Transforms a normalized Plane by a Quaternion rotation.
    /// @param plane  The normalized Plane to transform.
    /// This Plane must already be normalized, so that its Normal vector is of unit length, before this method is called.
    /// @param rotation The Quaternion rotation to apply to the Plane.
    /// @return A new Plane that results from applying the rotation.
    static Plane Transform(const Plane& plane, const Quaternion& rotation);

    /// Calculates the dot product of a Plane and Vector4.
    /// @param plane The Plane.
    /// @param value The Vector4.
    /// @return The dot product.
    static value_type Dot(const Plane& plane, const Vector4& value)
    {
        return plane.Normal.X * value.X +
            plane.Normal.Y * value.Y +
            plane.Normal.Z * value.Z +
            plane.D * value.W;
    }

    /// Returns the dot product of a specified Vector3 and the normal vector of this Plane plus the distance (D) value of the Plane.
    /// @param plane The Plane.
    /// @param value The Vector3.
    /// @return The resulting value.
    static value_type DotCoordinate(const Plane& plane, const Vector3& value)
    {
        return Vector3::Dot(plane.Normal, value) + plane.D;
    }

    /// Returns the dot product of a specified Vector3 and the Normal vector of this Plane.
    /// @param plane The Plane.
    /// @param value The Vector3.
    /// @return The resulting dot product.
    static value_type DotNormal(Plane plane, Vector3 value)
    {
        return Vector3::Dot(plane.Normal, value);
    }

public: /// operator override

    /// Returns a boolean indicating whether the two given Planes are equal.
    /// @param right The Plane to compare.
    /// @return True if the Planes are equal; False otherwise.
    inline bool operator ==(Plane right)const
    {
        return this->Equals(right);
    }

    /// Returns a boolean indicating whether the two given Planes are not equal.
    /// @param right The Plane to compare.
    /// @return True if the Planes are not equal; False if they are equal.
    inline bool operator !=( Plane right)
    {
        return !((*this) == right);
    }
};

NS_FK_END

#endif // END OF FOUNDATIONKIT_PLANE_HPP