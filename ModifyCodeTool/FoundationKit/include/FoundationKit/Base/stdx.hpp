/****************************************************************************
  Copyright (c) 2017 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_STDX_HPP
#define FOUNDATIONKIT_STDX_HPP

#include <mutex>
#include <tuple>
#include "FoundationKit/GenericPlatformMacros.hpp"

namespace std
{
#if CPP_CURRENT_VERSION < CPP_VERSION_17
    // Impl std::apply
    namespace detail
    {
        // TEMPLATE FUNCTION apply_impl
        template <class F, class Tuple, std::size_t... I>
        constexpr decltype(auto) apply_impl(F &&f, Tuple &&t, std::index_sequence<I...>)
        {
            //return std::invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...); // C++ 17
            return std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...);
        }
}  // namespace detail

    template <class F, class Tuple>
    constexpr decltype(auto) apply(F &&f, Tuple &&t)
    {
        return detail::apply_impl(
            std::forward<F>(f), std::forward<Tuple>(t),
            std::make_index_sequence<std::tuple_size<typename std::remove_reference<Tuple>::type>::value>{});
    }

#else
    /// std::apply Defined in header <tuple> && since C++17
#endif

#if CPP_CURRENT_VERSION < CPP_VERSION_17
    namespace detail {
        template <class T, class Tuple, std::size_t... I>
        constexpr T make_from_tuple_impl(Tuple&& t, std::index_sequence<I...>)
        {
            return T(std::get<I>(std::forward<Tuple>(t))...);
    }
} // namespace detail

    template <class T, class Tuple>
    constexpr T make_from_tuple(Tuple&& t)
    {
        return detail::make_from_tuple_impl<T>(std::forward<Tuple>(t),
                                               std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
    }
#else
    /// std::make_from_tuple Defined in header <tuple> && since C++17
#endif

#if CPP_CURRENT_VERSION < CPP_VERSION_17
    template<class... _Mutexes>
    using scoped_lock = std::lock_guard<_Mutexes...>;
#else

    template<class... _Mutexes>
    using scoped_lock = std::scoped_lock<_Mutexes...>;
#endif

}

#endif // END OF FOUNDATIONKIT_STDX_HPP



