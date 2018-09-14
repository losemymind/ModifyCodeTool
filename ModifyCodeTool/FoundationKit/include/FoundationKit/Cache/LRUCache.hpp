/****************************************************************************
  Copyright (c) 2018 libo All rights reserved.
  
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_LRUCACHE_HPP
#define FOUNDATIONKIT_LRUCACHE_HPP

#include <mutex>
#include <unordered_map>
#include "FoundationKit/Cache/AbstractCache.hpp"
#include "FoundationKit/Cache/LRUStrategy.hpp"
NS_FK_BEGIN
/// An LRUCache implements Least Recently Used caching. The default size for a cache is 1024 entries.
template < typename TKey, typename TValue, typename TMap=std::unordered_map<TKey, TValue>, typename TMutex = std::mutex>
class LRUCache: public AbstractCache<TKey, TValue, LRUStrategy<TKey, TValue>, TMap,TMutex>

{
public:
	LRUCache(std::size_t size = 1024):
		AbstractCache<TKey, TValue, LRUStrategy<TKey, TValue>, TMap, TMutex>(LRUStrategy<TKey, TValue>(size))
	{
	}

	~LRUCache()
	{
	}
	LRUCache(const LRUCache& aCache) = delete;
	LRUCache& operator = (const LRUCache& aCache) = delete;
};
NS_FK_END

#endif // END OF FOUNDATIONKIT_LRUCACHE_HPP
