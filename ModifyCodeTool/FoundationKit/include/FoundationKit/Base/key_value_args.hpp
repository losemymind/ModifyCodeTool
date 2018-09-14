/****************************************************************************
  Copyright (c) 2018 libo All rights reserved.
  
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_KEY_VALUE_ARGS_HPP
#define FOUNDATIONKIT_KEY_VALUE_ARGS_HPP

#include "FoundationKit/GenericPlatformMacros.hpp"
NS_FK_BEGIN
/// Simply event arguments class to transfer a key and a value via an event call.
/// Note that key and value are *NOT* copied, only references to them are stored.
template <class TKey, class TValue>
class key_value_args
{
public:

    key_value_args(const TKey& aKey, const TValue& aVal) :
        _key(aKey),
        _value(aVal)
    {
    }

    key_value_args(const key_value_args& args) :
        _key(args._key),
        _value(args._value)
    {
    }

    ~key_value_args()
    {
    }

    key_value_args& operator = (const key_value_args& args) = delete;

    const TKey& key() const
        /// Returns a reference to the key,
    {
        return _key;
    }

    const TValue& value() const
        /// Returns a Reference to the value.
    {
        return _value;
    }

protected:
    const TKey&   _key;
    const TValue& _value;

};


template <class TKey>
class valid_args
{
public:
    valid_args(const TKey& key) :
        _key(key),
        _isValid(true)
    {
    }

    valid_args(const valid_args& args) :
        _key(args._key),
        _isValid(args._isValid)
    {
    }

    ~valid_args()
    {
    }

    valid_args& operator = (const valid_args& args) = delete;

    const TKey&	key() const
    {
        return _key;
    }

    bool isValid() const
    {
        return _isValid;
    }

    void invalidate()
    {
        _isValid = false;
    }

protected:
    const TKey& _key;
    bool        _isValid;
};

NS_FK_END
#endif // END OF FOUNDATIONKIT_KEY_VALUE_ARGS_HPP