/****************************************************************************
  Copyright (c) 2018 libo All rights reserved.
  
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_FUNCTIONAL_HPP
#define FOUNDATIONKIT_FUNCTIONAL_HPP


#include <type_traits>
#include <functional>

namespace stdx
{

    // TEMPLATE CLASS enable_if
    template < bool _Test, class _Ty = void >
    using disable_if = std::enable_if < !_Test, _Ty >;

    template <class T>
    inline void hash_combine(std::size_t& seed, const T& v)
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
}

#endif // END OF FOUNDATIONKIT_FUNCTIONAL_HPP