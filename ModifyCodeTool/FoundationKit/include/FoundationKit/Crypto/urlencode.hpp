/****************************************************************************
  Copyright (c) 2017 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_URLENCODE_HPP
#define FOUNDATIONKIT_URLENCODE_HPP

#include <string>
#include "FoundationKit/GenericPlatformMacros.hpp"
NS_FK_BEGIN
namespace URL
{
static inline std::string encode(const std::string& source)
{
    std::string result;
    result.reserve(3 * source.size());
    for (std::string::const_iterator i = source.begin(); i != source.end(); ++i)
    {
        if (isalnum(std::char_traits<std::string::value_type>::to_int_type(*i)))
        {
            result.push_back(*i);
            continue;
        }
        switch (*i)
        {
        case static_cast<std::string::value_type>('-') :
        case static_cast<std::string::value_type>('_') :
        case static_cast<std::string::value_type>('.') :
        case static_cast<std::string::value_type>('!') :
        case static_cast<std::string::value_type>('~') :
        case static_cast<std::string::value_type>('*') :
        case static_cast<std::string::value_type>('(') :
        case static_cast<std::string::value_type>(')') :
        case static_cast<std::string::value_type>('\'') :
                                                        result.push_back(*i);
            break;
        default:
            std::string::value_type bytes[3] = { '%', static_cast<char>((*i & 0xF0) / 16), static_cast<char>(*i & 0x0F) };
            for (std::size_t j = 1; j < sizeof(bytes); ++j)
            {
                bytes[j] += (bytes[j] > 9) ? 'A' - 10 : '0';
            }
            result.insert(result.end(), bytes, bytes + sizeof(bytes));
            break;
        }
    }
    return result;
}

static inline std::string decode(const std::string& source)
{
    std::string result;
    result.reserve(source.size());
    for (std::string::const_iterator i = source.begin(); i != source.end(); ++i)
    {
        switch (*i) {
        case static_cast<std::string::value_type>('+') :
            result.push_back(static_cast<std::string::value_type>(' '));
            break;
        case static_cast<std::string::value_type>('%') :
            if (std::distance(i, source.end()) > 2)
            {
                std::string::const_iterator f = i, s = i;
                std::advance(f, 1);
                std::advance(s, 2);
                std::string::value_type first = *f, second = *s;
                int digit = (first >= 'A' ? ((first & 0xDF) - 'A') + 10 : (first - '0')) * 16;
                digit += (second >= 'A') ? ((second & 0xDF) - 'A') + 10 : (second - '0');
                result.push_back(static_cast<std::string::value_type>(digit));
                std::advance(i, 2);
            }
            else {
                result.push_back('%');
            }
            break;
        default:
            result.push_back(*i);
            break;
        }
    }
    return result;
}

static inline bool IsURLEncode(const std::string& Payload)
{
    static char AllowedChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~";
    static bool bTableFilled = false;
    static bool AllowedTable[256] = { false };

    if (!bTableFilled)
    {
        for (int32 Idx = 0; Idx < (sizeof(AllowedChars)/sizeof(AllowedChars[0])) - 1; ++Idx)	// -1 to avoid trailing 0
        {
            uint8 AllowedCharIdx = static_cast<uint8>(AllowedChars[Idx]);
            AllowedTable[AllowedCharIdx] = true;
        }

        bTableFilled = true;
    }

    const size_t Num = Payload.size();
    for (size_t Idx = 0; Idx < Num; ++Idx)
    {
        if (!AllowedTable[Payload[Idx]])
            return false;
    }

    return true;
}

} /// namespace URL

NS_FK_END

#endif // FOUNDATIONKIT_URLENCODE_HPP



