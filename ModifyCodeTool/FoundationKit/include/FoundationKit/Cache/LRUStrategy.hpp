/****************************************************************************
  Copyright (c) 2018 libo All rights reserved.
  
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_LRUSTRATEGY_HPP
#define FOUNDATIONKIT_LRUSTRATEGY_HPP
#include <list>
#include <unordered_map>
#include "FoundationKit/Cache/AbstractStrategy.hpp"
NS_FK_BEGIN
/// An LRUStrategy implements least recently used cache replacement.
template <typename TKey, typename TValue>
class LRUStrategy: public AbstractStrategy<TKey, TValue>

{
public:
	typedef std::list<TKey>                    Keys;
	typedef typename Keys::iterator            Iterator;
	typedef typename Keys::const_iterator      ConstIterator;
	typedef std::unordered_map<TKey, Iterator> KeyIndex;
	typedef typename KeyIndex::iterator        IndexIterator;
	typedef typename KeyIndex::const_iterator  ConstIndexIterator;

public:
	LRUStrategy(std::size_t size):
		_size(size)
	{
		if (_size < 1) throw InvalidArgumentException("size must be > 0");
	}

	virtual~LRUStrategy()
	{
	}

	void OnAdd(const key_value_args <TKey, TValue>& args) override
	{
		_keys.push_front(args.key());
		std::pair<IndexIterator, bool> stat = _keyIndex.insert(std::make_pair(args.key(), _keys.begin()));
		if (!stat.second)
		{
			stat.first->second = _keys.begin();
		}
	}

	void OnRemove(const TKey& key) override
	{
		IndexIterator it = _keyIndex.find(key);

		if (it != _keyIndex.end())
		{
			_keys.erase(it->second);
			_keyIndex.erase(it);
		}
	}

	void OnGet(const TKey& key) override
	{
		// LRU: in case of an hit, move to begin
		IndexIterator it = _keyIndex.find(key);

		if (it != _keyIndex.end())
		{
			_keys.splice(_keys.begin(), _keys, it->second); //_keys.erase(it->second)+_keys.push_front(key);
			it->second = _keys.begin();
		}
	}

	void OnClear() override
	{
		_keys.clear();
		_keyIndex.clear();
	}

	void OnIsValid(valid_args<TKey>& args) override
	{
		if (_keyIndex.find(args.key()) == _keyIndex.end())
		{
			args.invalidate();
		}
	}

	void OnReplace(std::set<TKey>& elemsToRemove) override
	{
		// Note: replace only informs the cache which elements
		// it would like to remove!
		// it does not remove them on its own!
		std::size_t curSize = _keyIndex.size();

		if (curSize < _size)
		{
			return;
		}

		std::size_t diff = curSize - _size;
		Iterator it = --_keys.end(); //--keys can never be invoked on an empty list due to the minSize==1 requirement of LRU
		std::size_t i = 0;

		while (i++ < diff)
		{
			elemsToRemove.insert(*it);
			if (it != _keys.begin())
			{
				--it;
			}
		}
	}

protected:
	std::size_t _size;     /// Number of keys the cache can store.
	Keys        _keys;
	KeyIndex    _keyIndex; /// For faster access to _keys
};
NS_FK_END

#endif // END OF FOUNDATIONKIT_LRUSTRATEGY_HPP