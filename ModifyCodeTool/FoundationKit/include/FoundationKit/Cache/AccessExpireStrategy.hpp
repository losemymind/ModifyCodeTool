/****************************************************************************
  Copyright (c) 2018 libo All rights reserved.
  
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_ACCESSEXPIRESTRATEGY_HPP
#define FOUNDATIONKIT_ACCESSEXPIRESTRATEGY_HPP

#include "FoundationKit/Cache/ExpireStrategy.hpp"
NS_FK_BEGIN
/// An AccessExpireStrategy implements time and access based expiration of cache entries
template <typename TKey, typename TValue>
class AccessExpireStrategy : public ExpireStrategy<TKey, TValue>
{
public:
    /// Create an expire strategy. Note that the smallest allowed caching time is 25ms.
    /// Anything lower than that is not useful with current operating systems.
    AccessExpireStrategy(int64 ExpireTimeInMilliseconds) : ExpireStrategy<TKey, TValue>(ExpireTimeInMilliseconds)
    {
    }

    ~AccessExpireStrategy()
    {
    }

    void onGet(const void*, const TKey& key)
    {
        // get triggers an update to the expiration time
        typename ExpireStrategy<TKey, TValue>::KeysIterator it = this->Keys.find(key);
        if (it != this->Keys.end())
        {
            this->TimeKeyIndex.erase(it->second);
            auto itIdx = this->TimeKeyIndex.insert(std::make_pair(DeltaTimer(), key));
            it->second = itIdx;
        }
    }
};

NS_FK_END

#endif // END OF FOUNDATIONKIT_ACCESSEXPIRESTRATEGY_HPP
