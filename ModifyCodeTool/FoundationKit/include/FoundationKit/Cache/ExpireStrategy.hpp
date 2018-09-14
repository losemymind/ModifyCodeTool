/****************************************************************************
  Copyright (c) 2018 libo All rights reserved.
  
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_EXPIRESTRATEGY_HPP
#define FOUNDATIONKIT_EXPIRESTRATEGY_HPP


#include <map>
#include <unordered_map>
#include <chrono>
#include "FoundationKit/Cache/AbstractStrategy.hpp"
#include "FoundationKit/Foundation/DeltaTimer.hpp"

NS_FK_BEGIN
template<typename TKey, typename TValue>
class ExpireStrategy : public AbstractStrategy<TKey, TValue>
{
public:
    typedef std::multimap<DeltaTimer, TKey>           TimeKeyMap;
    typedef typename TimeKeyMap::iterator             TimeKeyIterator;
    typedef typename TimeKeyMap::const_iterator       ConstTimeKeyIterator;
    typedef std::unordered_map<TKey, TimeKeyIterator> KeysMap;
    typedef typename KeysMap::iterator                KeysIterator;
    typedef std::chrono::milliseconds                 TimeValueType;

public:
    ExpireStrategy(int64 ExpireTimeInMilliseconds)
        : ExpireMilliseconds(ExpireTimeInMilliseconds)
    {
        
    }

    virtual~ExpireStrategy() {}

    void OnAdd(const key_value_args <TKey, TValue>& args) override
    {
        TimeKeyIterator iter = TimeKeyIndex.insert(std::make_pair(DeltaTimer(), args.key()));
        std::pair<KeysIterator, bool> Status = Keys.insert(std::make_pair(args.key(), iter));
        if (!Status.second)
        {
            TimeKeyIndex.erase(Status.first->second);
            Status.first->second = iter;
        }
    }

    void OnRemove(const TKey& key) override
    {
        KeysIterator iter = Keys.find(key);
        if (iter != Keys.end())
        {
            TimeKeyIndex.erase(iter->second);
            Keys.erase(iter);
        }
    }

    void OnGet(const TKey& key) override
    {

    }

    void OnClear() override
    {
        Keys.clear();
        TimeKeyIndex.clear();
    }

    void OnIsValid(valid_args<TKey>& args) override
    {
        KeysIterator iter = Keys.find(args.key());
        if (iter != Keys.end())
        {
            if (iter->second->first.IsElapsed(std::chrono::milliseconds(ExpireMilliseconds)))
            {
                args.invalidate();
            }
        }
        else
        {
            args.invalidate();
        }
    }

    void OnReplace(std::set<TKey>& elemsToRemove) override
    {
        TimeKeyIterator iter = TimeKeyIndex.begin();
        while (iter != TimeKeyIndex.end() && iter->first.IsElapsed(std::chrono::milliseconds(ExpireMilliseconds)))
        {
            elemsToRemove.insert(iter->second);
            ++iter;
        }
    }

protected:
    int64           ExpireMilliseconds;
    KeysMap         Keys;
    TimeKeyMap      TimeKeyIndex;

};



NS_FK_END


#endif // END OF FOUNDATIONKIT_EXPIRESTRATEGY_HPP
