﻿/****************************************************************************
Copyright (c) 2017 libo All rights reserved.

losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_MATH_HPP
#define FOUNDATIONKIT_MATH_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <vector>
#include <limits>
#include <cmath>
#include <cfloat>
#include <random>
#include <algorithm>
#include <type_traits>
#include "FoundationKit/GenericPlatformMacros.hpp"
#include "FoundationKit/Math/MathContent.hpp"

NS_FK_BEGIN

/// Provides constants and static methods for trigonometric, 
/// logarithmic and other common mathematical functions.
/// https://github.com/mosra/magnum/tree/master/src/Magnum/Math
namespace Math
{
    /// returns the maximum of two values
    template< typename T >
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
    Max(T v1, T v2)
    {
        return v1 > v2 ? v1 : v2;
    }

    /// returns the minimum of two values
    template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type >
    FORCEINLINE T Min(T v1, T v2)
    {
        return v1 < v2 ? v1 : v2;
    }

    /// Returns the absolute value of val.
    template< typename T>
    FORCEINLINE T Abs(T val, typename std::enable_if< std::is_arithmetic<T>::value>::type* = 0)
    {
        return std::abs(val);
    }

    /// Returns the arc-cosine of val - the angle in radians whose cosine is val.
    template< typename T>
    FORCEINLINE T Acos(T val)
    {
        return std::acos(val);
    }

    /// Returns the arc-sine of val - the angle in radians whose sine is val.
    template< typename T >
    FORCEINLINE T Asin(const T val)
    { 
        return std::asin(val);
    }

    /// Returns the arc-tangent of val - the angle in radians whose tangent is val.
    template< typename T >
    FORCEINLINE T Atan(const T val)
    { 
        return std::atan(val); 
    }

    /// Returns the angle in radians whose Tan is y/x.
    template< typename T >
    FORCEINLINE T Atan2(const T y, const T x)
    { 
        return std::atan2(y, x); 
    }

    /// Returns the smallest integer greater to or equal to val.
    template< typename T >
    FORCEINLINE T Ceil(const T val)
    { 
        return std::ceil(val); 
    }

    /// Returns the smallest integer greater to or equal to val.
    template< typename T >
    FORCEINLINE int CeilToInt(const T val)
    {
        return static_cast<int>(std::ceil(val)); 
    }

    /// Returns the largest integer smaller to or equal to val.
    template< typename T >
    FORCEINLINE T Floor(T val)
    {
        return std::floor(val);
    }

    /// Returns the largest integer smaller to or equal to val.
    template< typename T >
    FORCEINLINE int FloorToInt(T val)
    {
        return static_cast<int>(std::floor(val));
    }

    /// Returns f rounded to the nearest integer.
    template <typename T>
    FORCEINLINE typename std::enable_if< std::is_arithmetic<T>::value, T >::type
        Round(T val)
    {
#if PLATFORM_ANDROID
        return (val > T(0)) ? std::floor(val + T(0.5)) : std::ceil(val - T(0.5));
#else
        return std::round(val);
#endif
    }

    /// Clamps val value between a minimum and maximum value.
    template< typename T>
    FORCEINLINE T Clamp(T val, T minVal, T maxVal)
    {
        return std::min(std::max(val, minVal), maxVal);
    }

    /// Clamps value between 0 and 1 and returns value.
    template< typename T>
    FORCEINLINE T Clamp01(T val)
    { 
        return std::min(std::max(val, 0), 1);
    }

    /// Returns the cosine of angle f in radians.
    template< typename T >
    FORCEINLINE T Cos(T val)
    { 
        return std::cos(val);
    }

    /// Returns e raised to the specified power.
    template< typename T >
    FORCEINLINE T Exp(T power)
    { 
        return std::exp(power);
    }

    /// Returns the natural (base e) logarithm of val specified value.
    template< typename T >
    FORCEINLINE T Log(T val)
    {
        return std::log(val);
    }

    /// Returns the base 10 logarithm of val specified value.
    template< typename T >
    FORCEINLINE T Log10(T val0)
    {
        return std::log10(val0);
    }


    /// Returns the sign of val.
    template <typename T>
    FORCEINLINE T Sign(T val)
    {
        return (val >(T)0) ? (T)1 : ((val < (T)0) ? (T)-1 : (T)0);
    }

    /// Returns the sine of angle val in radians.
    template <typename T>
    FORCEINLINE T Sin(T val)
    {
        return std::sin(val);
    }

    /// Returns square root of val.
    template <typename T>
    FORCEINLINE T Sqrt(T val)
    {
        return std::sqrt(val);
    }

    /// Returns the tangent of angle val in radians.
    template <typename T>
    FORCEINLINE T Tan(T val)
    {
        return std::tan(val);
    }

    /// Returns f raised to power p.
    template< typename T, typename U >
    FORCEINLINE T Pow(T f, U p)
    {
        return std::pow(f, p);
    }

    // Linearly interpolate between range_start and range_end, based on percent.
    template <class T, class T2>
    FORCEINLINE T Lerp(const T &range_start, const T &range_end, const T2 &percent) 
    {
        const T2 one_minus_percent = static_cast<T2>(1.0) - percent;
        return range_start * one_minus_percent + range_end * percent;
    }

    // Linearly interpolate between range_start and range_end, based on percent.
    template <class T>
    FORCEINLINE T Lerp(const T &range_start, const T &range_end, const T &percent) 
    {
        return Lerp<T, T>(range_start, range_end, percent);
    }

    FORCEINLINE std::vector<double> Linspace(double begin, double end, size_t count)
    {
        std::vector<double> result(count, begin);
        if (count <=1 || std::fabs(begin-end) < std::numeric_limits<double>::epsilon())
        {
            return result;
        }
        for (size_t i = 0; i < count; ++i)
        {
            result[i] = begin + ((end - begin)*i) / (count - 1);
        }
        result[count - 1] = end;
        return result;
    }

   /**
    * Compares two floating point values if they are similar.
    * @param[in] v1 First value.
    * @param[in] v2 Second value.
    * @param[in] threshold The threshold of similarity
    * @return True if the values are similar, otherwise false.
    */
    template< typename T, typename U, typename V>
    FORCEINLINE bool RoughlyEqual(const T v1, const U v2, const V threshold = 0.01f)
    { 
        return std::abs(v1 - v2) <= threshold;  
    }

   /**
    * Compares two values if they are similar.
    * Due to floating point imprecision it is not
    * recommended to compare floats using the equal
    * operator. eg. 1.0 == 10.0 / 10.0 might not return true.
    */
    template< typename T1, typename T2 >
    FORCEINLINE bool Approximately(const T1 a, const T2 b)
    {
        return RoughlyEqual(a, b, 0.1f);
    }

    template< typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    FORCEINLINE bool Equal (T v1, T v2)
    {
        return v1 == v2;
    }

    FORCEINLINE bool Equal(float v1, float v2)
    {
        return (std::fabs(v1 - v2) < FLT_EPSILON /*1E-12*/);
    }

    FORCEINLINE bool Equal(double v1, double v2)
    {
        return (std::fabs(v1 - v2) < DBL_EPSILON /*1E-12*/);
    }

    template< typename T >
    FORCEINLINE bool FuzzyEqual(T v1, T v2, T var)
    {
        return(v1 - var <= v2 && v2 <= v1 + var);
    }

    /// Return true if the parameter is equal to zero
    FORCEINLINE bool IsZero(double val)
    {
        return ((-MinDouble < val) && (val < MinDouble));
    }

    /// Returns if the value is powered by two.
    FORCEINLINE bool IsPowerOfTwo(int val)
    {
        return (val > 0) && ((val & (val - 1)) == 0);
    }

    //g(z)=1/(1+e^-z)
    FORCEINLINE double Sigmoid(double input, double response = 1.0)
    {
        return (1.0 / (1.0 + std::exp(-input / response)));
    }

    FORCEINLINE std::vector<double> Sigmoid(std::vector<double> inputs, double response = 1.0)
    {
        std::vector<double> outputs;
        for (double val : inputs)
        {
            outputs.push_back((1.0 / (1.0 + std::exp(-val / response))));
        }
        return outputs;
    }

    // return true is the third parameter is in the range described by the first two
    template< typename T, typename U, typename V>
    FORCEINLINE bool InRange(T range_start, U range_end, V val)
    {
        if (range_start < range_end)
        {
            return ((val > range_start) && (val < range_end));
        }
        else
        {
            return ((val < range_start) && (val > range_end));
        }
    }


   /** Converts radians to degrees.
    *  This method uses double precission internally,
    *  though it returns single float
    *  Factor = 180 / pi
    *  @param[in] radians The angle in radians.
    *  @return    The angle in degrees.
    */
    FORCEINLINE float RadiansToDegrees(float radians)
    {
        return static_cast<float>(radians * 57.295779513082320876798154814105);
    }

   /** Converts degrees to radians.
    *  This method uses double precission internally,
    *  though it returns single float
    *  Factor = pi / 180
    *  @param[in]  degrees  The angle in degrees.
    *  @return     The angle in radians.
    */
    FORCEINLINE float DegreesToRadians(float degrees)
    {
        return static_cast<float>(degrees * 0.017453292519943295769236907684886);
    }

    template<typename T>
    double Average(const std::vector<T>& vec,
        typename std::enable_if< std::is_arithmetic<T>::value >::type* = nullptr)
    {
        double dAverage = 0.0;
        for (auto& i : vec )
        {
            dAverage += static_cast<double>(i);
        }
        return dAverage / static_cast<double>(vec.size());
    }

    /// Get the next power of two after val value.
    int NextPowerOfTwo(int val);

    /// Returns the closest power of two to val value.
    int ClosestPowerOfTwo(int val);

    double StandardDeviation(const std::vector<double>& vec);

    std::default_random_engine& GetRandomEngine();

   /**
    * Random number distribution that produces floating-point values
    *        according to a uniform distribution, which is described
    *        by the following probability density function:
    *        p(x|a,b)=1/b-a, a ≤ x < b
    *        if want to get a floating-point number in the range[0.0, 1.0],
    *        use random<double>(0.0, std::nextafter(1, DBL_MAX));
    * The random loating_point number in a range [a, b)
    * @param  a and b are set on construction.
    * @return A floating-point type, by default type is double.
    */
    template< class _Ty >
    static inline _Ty Random(_Ty a, _Ty b,  typename std::enable_if< std::is_floating_point<_Ty>::value >::type* = 0)
    {
        std::uniform_real_distribution<_Ty> randomResult(a, b);
        return randomResult(GetRandomEngine());
    }

   /**
    * Get random integral number in a range [a, b]
    * @return An integer type, by default the type is int.
    */
    template< class _Ty >
    static inline _Ty Random(_Ty a, _Ty b, typename std::enable_if< std::is_integral<_Ty>::value >::type* = 0)
    {
        std::uniform_int_distribution<_Ty> randomResult(a, b);
        return randomResult(GetRandomEngine());
    }

   /**
    * @return A random floating-point number in the range [0, 1.0].
    */
    template< class _Ty >
    static inline _Ty Random(typename std::enable_if< std::is_floating_point<_Ty>::value >::type* = 0)
    {
        return Random<_Ty>((_Ty)0, /*std::*/nextafter(1, DBL_MAX));
    }

    // return a random boolean.
    static inline bool   RandBoolean()
    {
        std::bernoulli_distribution booleanDist;
        return booleanDist(GetRandomEngine());
        // or
        //return Random<double>() > 0.5;
    }


    //returns a random double in the range -1 < n < 1
    static inline double RandClamped()
    {
        return Random<double>() - Random<double>();
    }

} // namespace Math

NS_FK_END
#endif // FOUNDATIONKIT_MATH_HPP