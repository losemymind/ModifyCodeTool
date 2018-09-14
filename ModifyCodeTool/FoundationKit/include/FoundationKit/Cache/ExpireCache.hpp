/****************************************************************************
  Copyright (c) 2018 libo All rights reserved.
  
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_EXPIRECACHE_HPP
#define FOUNDATIONKIT_EXPIRECACHE_HPP
#include <mutex>
#include <unordered_map>
#include "FoundationKit/Cache/AbstractCache.hpp"
#include "FoundationKit/Cache/ExpireStrategy.hpp"
NS_FK_BEGIN
/// An ExpireCache caches entries for a fixed time period (per default 5 minutes).
/// Be careful when using an ExpireCache. A cache is often used
/// like cache.Has(x) followed by cache.Het x). Note that it could happen
/// that the "Has" call works, then the current execution thread gets descheduled, time passes,
/// the entry gets invalid, thus leading to an TValue() being returned
/// when "Get" is invoked.
template <typename TKey,typename TValue,typename TMap = std::unordered_map<TKey, TValue>, typename TMutex = std::mutex>
class ExpireCache : public AbstractCache<TKey, TValue, ExpireStrategy<TKey, TValue>, TMap, TMutex>
{
public:
    ExpireCache(int64 ExpireTimeInMilliseconds = 5*60*1000) :
        AbstractCache<TKey, TValue, ExpireStrategy<TKey, TValue>, TMap, TMutex>(ExpireStrategy<TKey, TValue>(ExpireTimeInMilliseconds))
    {
    }

    ~ExpireCache()
    {
    }
    ExpireCache(const ExpireCache& aCache)=delete;
    ExpireCache& operator = (const ExpireCache& aCache)=delete;

};
NS_FK_END
#endif // END OF FOUNDATIONKIT_EXPIRECACHE_HPP