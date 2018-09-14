/****************************************************************************
  Copyright (c) 2017 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_ARGUMENT_FILTER_HPP
#define FOUNDATIONKIT_ARGUMENT_FILTER_HPP

#include <tuple>
template< typename T>
struct Filter
{
    static constexpr auto func()
    {
        return std::tuple();
    }

    template< class... Args >
    static constexpr auto func(T&&, Args&&...args)
    {
        return Filter::func(std::forward<Args>(args)...);
    }

    template< class X, class... Args >
    static constexpr auto func(X&&x, Args&&...args)
    {
        return std::tuple_cat(std::make_tuple(std::forward<X>(x)), Filter::func(std::forward<Args>(args)...));
    }
};

//test code
//auto tp1 = Filter<bool>::func(true, 2, false, 2.5);//tuple(2, 2.5)

#endif // END OF FOUNDATIONKIT_ARGUMENT_FILTER_HPP