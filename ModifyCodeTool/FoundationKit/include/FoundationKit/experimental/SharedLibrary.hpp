/****************************************************************************
  Copyright (c) 2017 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_SHAREDLIBRARY_HPP
#define FOUNDATIONKIT_SHAREDLIBRARY_HPP

#include "FoundationKit/GenericPlatformMacros.hpp"
#include <string>
#include <functional>
#if PLATFORM_WINDOWS
#define ModuleHandle  HMODULE
#define OpenLibrary   LoadLibraryA
#define GetMethod     GetProcAddress
#define CloseLibrary(LIB) FreeLibraryAndExitThread(LIB, 0)
#else
#include <dlfcn.h>
#define ModuleHandle  void*
#define OpenLibrary   dlopen
#define GetMethod     dlsym
#define CloseLibrary dlclose
#endif

class SharedLibrary
{
public:
    SharedLibrary() :Handle(nullptr) {}
    SharedLibrary(const std::string& LibraryName)
        :Handle(nullptr)
    {
        Handle = OpenLibrary(LibraryName.c_str());
    }
    ~SharedLibrary()
    {
        CloseLibrary(Handle);
    }

    template <typename _Fty>
    std::function<_Fty> GetFunction(const char* FuncName)
    {
        auto MethodAddr = GetMethod(Handle, FuncName);
        if (!MethodAddr)
            return nullptr;
        return std::function<_Fty>((_Fty*)(MethodAddr));
    }

    template <typename _Fty, typename... Args>
    typename std::result_of<std::function<_Fty>(Args...)>::type Call(const string& FuncName, Args&&... args)
    {
        auto Func = GetFunction<_Fty>(FuncName);
        if (Func == nullptr)
        {
            string e = "can not find this function " + FuncName;
            throw std::exception(e.c_str());
        }
        return Func(std::forward<Args>(args)...);
    }
private:
    ModuleHandle Handle;
};

//SharedLibrary MainModule("Main.dll");
//auto Result = MainModule.Call<int(int, const char*)>("Func", 10, "asdsdf");

#endif // END OF FOUNDATIONKIT_SHAREDLIBRARY_HPP
