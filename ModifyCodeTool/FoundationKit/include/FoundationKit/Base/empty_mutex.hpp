/****************************************************************************
  Copyright (c) 2018 libo All rights reserved.
  
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_EMPTY_MUTEX_HPP
#define FOUNDATIONKIT_EMPTY_MUTEX_HPP
#include "FoundationKit/GenericPlatformMacros.hpp"
NS_FK_BEGIN
class empty_mutex
{
public:

    empty_mutex(const empty_mutex&) = delete;
    empty_mutex& operator = (const empty_mutex&) = delete;
    /// Destroys the Mutex.
    ~empty_mutex() {};
    void lock() {};
    bool tryLock() {};
    void unlock() {};

};
NS_FK_END
#endif // END OF FOUNDATIONKIT_EMPTY_MUTEX_HPP