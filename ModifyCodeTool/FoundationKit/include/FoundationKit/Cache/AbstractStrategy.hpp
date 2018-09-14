/****************************************************************************
  Copyright (c) 2018 libo All rights reserved.
  
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_ABSTRACTSTRATEGY_HPP
#define FOUNDATIONKIT_ABSTRACTSTRATEGY_HPP

#include "FoundationKit/GenericPlatformMacros.hpp"
#include "FoundationKit/Base/key_value_args.hpp"
#include <set>
NS_FK_BEGIN
template <typename TKey, typename TValue>
class AbstractStrategy
	/// An AbstractStrategy is the interface for all strategies.
{
public:
	AbstractStrategy()
	{
	}

	virtual ~AbstractStrategy()
	{
	}

	/// Updates an existing entry.
	virtual void OnUpdate(const key_value_args <TKey, TValue>& args)
	{
		OnRemove(args.key());
		OnAdd(args);
	}
	
	/// Adds the key to the strategy.
	/// If for the key already an entry exists, an exception will be thrown.
	virtual void OnAdd(const key_value_args <TKey, TValue>& key) = 0;

	/// Removes an entry from the strategy. If the entry is not found
	/// the remove is ignored.
	virtual void OnRemove(const TKey& key) = 0;

	/// Informs the strategy that a read-access happens to an element.
	virtual void OnGet(const TKey& key) = 0;

	/// Removes all elements from the cache.
	virtual void OnClear() = 0;

	/// Used to query if a key is still valid (i.e. cached).
	virtual void OnIsValid(valid_args<TKey>& key) = 0;

	/// Used by the Strategy to indicate which elements should be removed from
	/// the cache. Note that onReplace does not change the current list of keys.
	/// The cache object is responsible to remove the elements.
	virtual void OnReplace(std::set<TKey>& elemsToRemove) = 0;

};
NS_FK_END
#endif // END OF FOUNDATIONKIT_ABSTRACTSTRATEGY_HPP
