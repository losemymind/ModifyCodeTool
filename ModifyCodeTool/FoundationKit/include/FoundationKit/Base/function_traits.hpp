/****************************************************************************
  Copyright (c) 2017 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_FUNCTION_TRAITS_HPP
#define FOUNDATIONKIT_FUNCTION_TRAITS_HPP

#pragma once
#include <tuple>
#include <functional>
#include <type_traits>
#include "FoundationKit/GenericPlatformMacros.hpp"
NS_FK_BEGIN

/// TEMPLATE CLASS function_traits_impl
template <typename T>
struct function_traits_impl;

/// TEMPLATE CLASS function_traits
template<typename _Ft>
struct function_traits : function_traits_impl <std::remove_cv_t<std::remove_reference_t<_Ft>>>
{
};

template<typename ReturnType, typename... Args>
struct function_traits_impl<ReturnType(Args...)>
{
public:
    typedef  ReturnType(*pointer)(Args...);
    typedef  ReturnType function_meta_type(Args...);
    using function_type      = std::function <function_meta_type>;
    using argument_meta_type = std::tuple<std::remove_cv_t<std::remove_reference_t<Args> >... >;
    using argument_types     = std::tuple < Args... > ;
    using return_type        = ReturnType;
    using arity              = std::integral_constant < unsigned, sizeof...(Args) > ;

    template<size_t I>
    struct argument
    {
        static_assert(I < arity::value, "index is out of range, index must less than sizeof Args");
        using type = typename std::tuple_element<I, argument_types>::type;
    };
};

/// function pointer
template<typename ReturnType, typename... Args>
struct function_traits_impl<ReturnType(*)(Args...)> : function_traits<ReturnType(Args...)>{};

/// std::function
template <typename ReturnType, typename... Args>
struct function_traits_impl<std::function<ReturnType(Args...)>> : function_traits_impl<ReturnType(Args...)>{};

///member function
#define FUNCTION_TRAITS(...)\
template <typename ReturnType, typename ClassType, typename... Args>\
struct function_traits_impl<ReturnType(ClassType::*)(Args...) __VA_ARGS__> : function_traits_impl<ReturnType(Args...)>{};

/// pointer of non-static member function
FUNCTION_TRAITS()
FUNCTION_TRAITS(const)
FUNCTION_TRAITS(volatile)
FUNCTION_TRAITS(const volatile)
#undef FUNCTION_TRAITS

/// functor
template<typename _CallableType>
struct function_traits_impl : function_traits_impl<decltype(&_CallableType::operator())> {};

template <typename _Ft>
typename function_traits<_Ft>::function_type to_function(const _Ft& lambda)
{
    return static_cast<typename function_traits<_Ft>::function_type>(lambda);
}

template <typename _Ft>
typename function_traits<_Ft>::function_type to_function(_Ft&& lambda)
{
    return static_cast<typename function_traits<_Ft>::function_type>(std::forward<_Ft>(lambda));
}

template <typename _Ft>
typename function_traits<_Ft>::pointer to_function_pointer(const _Ft& lambda)
{
    return static_cast<typename function_traits<_Ft>::pointer>(lambda);
}

/// Alias for the function result.
template <typename _Ft>
using function_result_t = typename function_traits<_Ft>::return_type;

/// Alias for the type of the argument list.
template <typename _Ft>
using function_arguments_t = typename function_traits<_Ft>::argument_types;

template <typename _Ft>
using function_meta_arguments_t = typename function_traits<_Ft>::argument_meta_type;

/// Alias for the type of the nth argument.
template <typename _Ft, std::size_t I>
using function_argument_t = typename std::tuple_element<I, typename function_traits<_Ft>::argument_types>::type;
//using function_argument_t = typename function_traits<_Ft>::argument<I>::type;

/// Alias for the class type the function is member to.
template <typename _Ft>
using function_t = typename function_traits<_Ft>::function_type;

NS_FK_END

#endif // FOUNDATIONKIT_FUNCTION_TRAITS_HPP


