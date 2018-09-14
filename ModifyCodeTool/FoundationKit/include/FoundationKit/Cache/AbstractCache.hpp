/****************************************************************************
  Copyright (c) 2018 libo All rights reserved.
  
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_ABSTRACTCACHE_HPP
#define FOUNDATIONKIT_ABSTRACTCACHE_HPP
#include <functional>
#include <unordered_map>
#include <set>
#include"FoundationKit/GenericPlatformMacros.hpp"
#include "FoundationKit/Base/empty_mutex.hpp"
#include "FoundationKit/Base/key_value_args.hpp"
NS_FK_BEGIN
/// An AbstractCache is the interface of all caches.
template <typename TKey, typename TValue, typename TStrategy, typename TMap = std::unordered_map<TKey, TValue>, typename TMutex = empty_mutex>
class AbstractCache

{
public:
	typedef typename TMap::iterator       Iterator;
	typedef typename TMap::const_iterator ConstIterator;
	typedef std::set<TKey>                KeySet;

	AbstractCache()
	{
		Initialize();
	}

	AbstractCache(const TStrategy& InStrategy): Strategy(InStrategy)
	{
		Initialize();
	}

	virtual ~AbstractCache()
	{

	}

	AbstractCache(const AbstractCache& aCache) = delete;
	AbstractCache& operator = (const AbstractCache& aCache) = delete;

	/// Adds the key value pair to the cache.
	/// If for the key already an entry exists, it will be overwritten.
	void Add(const TKey& key, const TValue& val)
	{
		std::lock_guard<TMutex> lock(DataMutex);
		DoAdd(key, val);
	}

	/// Adds the key value pair to the cache. Note that adding a NULL SharedPtr will fail!
	/// If for the key already an entry exists, it will be overwritten.
	/// The difference to add is that no remove or add events are thrown in this case,
	/// just a simply silent update is performed
	/// If the key does not exist the behavior is equal to add, ie. an add event is thrown
	void Update(const TKey& key, const TValue& val)
	{
		std::lock_guard<TMutex> lock(DataMutex);
		DoUpdate(key, val);
	}


	void Remove(const TKey& key)
		/// Removes an entry from the cache. If the entry is not found,
		/// the remove is ignored.
	{
		std::lock_guard<TMutex> lock(DataMutex);
		Iterator it = Data.find(key);
		DoRemove(it);
	}

	bool Has(const TKey& key) const
		/// Returns true if the cache contains a value for the key.
	{
		std::lock_guard<TMutex> lock(DataMutex);
		return DoHas(key);
	}

	/// Returns a SharedPtr of the value. The SharedPointer will remain valid
	/// even when cache replacement removes the element.
	/// If for the key no value exists, an empty SharedPtr is returned.
	TValue Get(const TKey& key)
	{
		std::lock_guard<TMutex> lock(DataMutex);
		return DoGet(key);
	}

	void Clear()
		/// Removes all elements from the cache.
	{
		std::lock_guard<TMutex> lock(DataMutex);
		DoClear();
	}

	std::size_t Size()
		/// Returns the number of cached elements
	{
		std::lock_guard<TMutex> lock(DataMutex);
		DoReplace();
		return Data.size();
	}

	/// Forces cache replacement. Note that Poco's cache strategy use for efficiency reason no background thread
	/// which periodically triggers cache replacement. Cache Replacement is only started when the cache is modified
	/// from outside, i.e. add is called, or when a user tries to access an cache element via get.
	/// In some cases, i.e. expire based caching where for a long time no access to the cache happens,
	/// it might be desirable to be able to trigger cache replacement manually.
	void ForceReplace()
	{
		std::lock_guard<TMutex> lock(DataMutex);
		DoReplace();
	}

	/// Returns a copy of all keys stored in the cache
	std::set<TKey> GetAllKeys()
	{
		std::lock_guard<TMutex> lock(DataMutex);
		DoReplace();
		ConstIterator it = Data.begin();
		ConstIterator itEnd = Data.end();
		std::set<TKey> result;
		for (; it != itEnd; ++it)
			result.insert(it->first);
		return result;
	}

protected:
	std::function<void(const key_value_args<TKey, TValue >)> OnAdd;
	std::function<void(const key_value_args<TKey, TValue >)> OnUpdate;
	std::function<void(const TKey)>                          OnRemove;
	std::function<void(const TKey)>                          OnGet;
	std::function<void()>                                    OnClear;
	mutable std::function<void(valid_args<TKey>&)>           OnIsValid;
	mutable std::function<void(KeySet&)>                     OnReplace;

	void Initialize()
	{

		OnAdd     = std::bind(&TStrategy::OnAdd, &Strategy, std::placeholders::_1);
		OnUpdate  = std::bind(&TStrategy::OnUpdate, &Strategy, std::placeholders::_1);
		OnRemove  = std::bind(&TStrategy::OnRemove, &Strategy, std::placeholders::_1);
		OnGet     = std::bind(&TStrategy::OnGet, &Strategy, std::placeholders::_1);
		OnClear   = std::bind(&TStrategy::OnClear, &Strategy);
		OnIsValid = std::bind(&TStrategy::OnIsValid, &Strategy, std::placeholders::_1);
		OnReplace = std::bind(&TStrategy::OnReplace, &Strategy, std::placeholders::_1);
	}

	/// Adds the key value pair to the cache.
	/// If for the key already an entry exists, it will be overwritten.
	void DoAdd(const TKey& key, const TValue& val)
	{
		Iterator it = Data.find(key);
		DoRemove(it);
		key_value_args<TKey, TValue> args(key, val);
		OnAdd(args);
		Data.insert(std::make_pair(key, val));
		DoReplace();
	}

	/// Adds the key value pair to the cache.
	/// If for the key already an entry exists, it will be overwritten.
	void DoUpdate(const TKey& key, const TValue& val)
	{
		key_value_args<TKey, TValue> args(key, val);
		Iterator it = Data.find(key);
		if (it == Data.end())
		{
			OnAdd(args);
			Data.insert(std::make_pair(key, val));
		}
		else
		{
			OnUpdate(args);
			it->second = val;
		}
		DoReplace();
	}

	/// Removes an entry from the cache. If the entry is not found
	/// the remove is ignored.
	void DoRemove(Iterator it)
	{
		if (it != Data.end())
		{
			OnRemove(it->first);
			Data.erase(it);
		}
	}

	/// Returns true if the cache contains a value for the key
	bool DoHas(const TKey& key) const
	{
		// ask the strategy if the key is valid
		ConstIterator it = Data.find(key);
		bool result = false;

		if (it != Data.end())
		{
			valid_args<TKey> args(key);
			OnIsValid(args);
			result = args.isValid();
		}
		return result;
	}

	/// Returns a SharedPtr of the cache entry, returns 0 if for
	/// the key no value was found
	TValue DoGet(const TKey& key)
	{
		Iterator it = Data.find(key);
		TValue result;

		if (it != Data.end())
		{	
			// inform all strategies that a read-access to an element happens
			OnGet(key);
			// ask all strategies if the key is valid
			valid_args<TKey> args(key);
			OnIsValid(args);

			if (!args.isValid())
			{
				DoRemove(it);
			}
			else
			{
				result = it->second;
			}
		}

		return result;
	}

	void DoClear()
	{
		OnClear();
		Data.clear();
	}

	void DoReplace()
	{
		std::set<TKey> delMe;
		OnReplace(delMe);
		// delMe contains the to be removed elements
		typename std::set<TKey>::const_iterator it    = delMe.begin();
		typename std::set<TKey>::const_iterator endIt = delMe.end();

		for (; it != endIt; ++it)
		{
			Iterator itH = Data.find(*it);
			DoRemove(itH);
		}
	}

	TStrategy       Strategy;
	mutable TMap    Data;
	mutable TMutex  DataMutex;
};

NS_FK_END

#endif // END OF FOUNDATIONKIT_ABSTRACTCACHE_HPP
