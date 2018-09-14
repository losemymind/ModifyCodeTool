/****************************************************************************
  Copyright (c) 2017 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_PLATFORMTLS_HPP
#define FOUNDATIONKIT_PLATFORMTLS_HPP

#include "FoundationKit/GenericPlatformMacros.hpp"
#include "FoundationKit/Base/types.hpp"
#if (PLATFORM_ANDROID)
#include "FoundationKit/Platform/Android/AndroidPlatformTLS.hpp"
#elif (PLATFORM_APPLE)
#include "FoundationKit/Platform/Apple/ApplePlatformTLS.hpp"
#elif (PLATFORM_WINDOWS)
#include "FoundationKit/Platform/Windows/WindowsPlatformTLS.hpp"
#elif (PLATFORM_LINUX)
#include "FoundationKit/Platform/Linux/LinuxPlatformTLS.hpp"
#elif (PLATFORM_SWITCH)
#include "FoundationKit/Platform/Switch/SwitchPlatformTLS.hpp"
#else
#error Unknown platform
#endif

NS_FK_BEGIN

static const uint32 TLS_INVALID = 0xFFFFFFFF;

template<typename _Ty>
class TLS final
{
public:
    typedef TLS<_Ty> _Myt;
    TLS(const _Myt&) = delete;
    void operator=(const _Myt&) = delete;

    TLS() : Idx(TLS_INVALID) {}
    TLS(_Ty* Obj):Idx(PlatformTLS::AllocTlsSlot())
    {
        PlatformTLS::SetTlsValue(Idx, Obj);
    }

    ~TLS() 
    { 
        if (PlatformTLS::IsValidTlsSlot(Idx)) 
        { 
            PlatformTLS::FreeTlsSlot(Idx); 
        } 
    }

    TLS(_Myt&& _Right)
    {
        Idx = _Right.Idx;
        _Right.Idx = TLS_INVALID;
    }
    _Myt& operator=(_Myt&& _Right)
    {
        TLS(std::move(_Right)).Swap(*this);
        return (*this);
    }

    void Swap(_Myt& _Other) _NOEXCEPT
    {	
        std::swap(Idx, _Other.Idx);
    }

    void Reset() _NOEXCEPT
    {	// release resource and convert to empty TLS object
        TLS().Swap(*this);
    }

    _Ty *Get() const _NOEXCEPT
    {	// return pointer to resource
        return (reinterpret_cast<_Ty*>(PlatformTLS::GetTlsValue(Idx)));
    }

    _Ty& operator*() const _NOEXCEPT
    {	// return reference to resource
        return (*this->Get());
    }

    _Ty *operator->() const _NOEXCEPT
    {	// return pointer to resource
        return (this->Get());
    }
private:
    uint32 Idx;
};
NS_FK_END


#endif // FOUNDATIONKIT_PLATFORMTLS_HPP



