/****************************************************************************
  Copyright (c) 2018 libo All rights reserved.
  
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_OPENLIBRARY_HPP
#define FOUNDATIONKIT_OPENLIBRARY_HPP

#include "FoundationKit/GenericPlatformMacros.hpp"
#include <string>
#if defined(__linux__) || defined(ANDROID) || defined(__ANDROID__) || defined(__QNXNTO__) || defined(__APPLE__)
#include <unistd.h>
#include <dlfcn.h>
#endif

NS_FK_BEGIN

namespace lib
{
#if PLATFORM_WINDOWS
    typedef HINSTANCE LIBTYPE;
#else
    typedef void* LIBTYPE;
#endif

    namespace detail
    {

#if PLATFORM_WINDOWS
        inline lib::LIBTYPE OpenLibrary(const char* pszPath)
        {
#if defined(_UNICODE)
            if (!pszPath)
            {
                FKLog("Path must be valid '%s'", pszPath);
                return nullptr;
            }

            // Get full path of executable
            wchar_t pszPathW[FILEPATH_MAX];

            // Convert char to wchar
            DWORD i = 0;

            for (i = 0; i <= strlen(pszPath); ++i)
            {
                pszPathW[i] = static_cast<wchar_t>(pszPath[i]);
            }

            pszPathW[i] = ' ';
            lib::LIBTYPE hostLib = LoadLibraryW(pszPathW);
#else
            lib::LIBTYPE hostLib = LoadLibraryA(pszPath);
#endif // #if defined(_UNICODE)
            if (!hostLib)
            {
                FKLog("Could not load host library '%s'", pszPath);
            }
            FKLog("Host library '%s' loaded", pszPath);
            return hostLib;
        }

        inline void CloseLibrary(lib::LIBTYPE hostLib)
        {
            FreeLibrary(hostLib);
        }

        inline void* GetLibraryFunction(lib::LIBTYPE hostLib, const char* pszName)
        {
            if (hostLib)
            {
#if defined(UNDER_CE)
                return win32::GetProcAddressA(hostLib, pszName);
#else
                return GetProcAddress(hostLib, pszName);
#endif // #if defined(UNDER_CE)
            }
            return nullptr;
        }

#else // #if PLATFORM_WINDOWS

#if PLATFORM_MAC
#include "CoreFoundation/CoreFoundation.h"
        static const char* g_pszEnvVar = "LIBO_LIB_PATH";
        inline void* OpenFramework(const char* pszPath)
        {
            CFBundleRef mainBundle = CFBundleGetMainBundle();
            CFURLRef resourceURL = CFBundleCopyPrivateFrameworksURL(mainBundle);
            char path[PATH_MAX];
            if (!CFURLGetFileSystemRepresentation(resourceURL, TRUE, (UInt8*)path, PATH_MAX))
            {
                return 0;
            }
            CFRelease(resourceURL);

            {
                void* lib = NULL;

                // --- Set a global environment variable to point to this path (for VFrame usage)
                const char* slash = strrchr(pszPath, '/');
                if (slash)
                {
                    char szPath[FILENAME_MAX];
                    memset(szPath, 0, sizeof(szPath));
                    strncpy(szPath, pszPath, slash - pszPath);
                    setenv(g_pszEnvVar, szPath, 1);
                }
                else
                {
                    // Use the current bundle path
                    std::string framework = std::string(path) + "/../Frameworks/";
                    setenv(g_pszEnvVar, framework.c_str(), 1);
                }

                // --- Make a temp symlink
                char szTempFile[FILENAME_MAX];
                memset(szTempFile, 0, sizeof(szTempFile));

                char tmpdir[PATH_MAX];
                size_t n = confstr(_CS_DARWIN_USER_TEMP_DIR, tmpdir, sizeof(tmpdir));
                if ((n <= 0) || (n >= sizeof(tmpdir)))
                {
                    strlcpy(tmpdir, getenv("TMPDIR"), sizeof(tmpdir));
                }

                strcat(szTempFile, tmpdir);
                strcat(szTempFile, "tmp.XXXXXX");

                if (mkstemp(szTempFile))
                {
                    if (symlink(pszPath, szTempFile) == 0)
                    {
                        lib = dlopen(szTempFile, RTLD_LAZY | RTLD_GLOBAL);
                        remove(szTempFile);
                    }
                }

                // --- Can't find the lib? Check the application framework folder instead.
                if (!lib)
                {
                    std::string framework = std::string(path) + std::string("/") + pszPath;
                    lib = dlopen(framework.c_str(), RTLD_LAZY | RTLD_GLOBAL);

                    if (!lib)
                    {
                        const char* err = dlerror();
                        if (err)
                        {
                            // NSLog(@"dlopen failed with error: %s => %@", err, framework);
                        }
                    }
                }

                return lib;
            }
        }

        inline lib::LIBTYPE OpenLibrary(const char* pszPath)
        {
            // An objective-C function that uses dlopen
            lib::LIBTYPE hostLib = OpenFramework(pszPath);
            if (!hostLib)
            {
                FKLog("Could not load host library '%s'", pszPath);
            }
            FKLog("Host library '%s' loaded", pszPath);
            return hostLib;
        }

#elif PLATFORM_IOS //#if PLATFORM_MAC
        inline lib::LIBTYPE OpenLibrary_Helper(const char* pszPath)
        {
            lib::LIBTYPE hostLib = dlopen(pszPath, RTLD_LAZY | RTLD_GLOBAL);
            if (!hostLib)
            {
                char pathMod[256];
                strcpy(pathMod, "./");
                strcat(pathMod, pszPath);

                hostLib = dlopen(pathMod, RTLD_LAZY | RTLD_GLOBAL);
            }
            return hostLib;
        }

        inline lib::LIBTYPE OpenLibrary(const char* pszPath)
        {
            size_t start = 0;
            std::string tmp;
            std::string LibPath(pszPath);
            lib::LIBTYPE hostLib = nullptr;

            while (!hostLib)
            {
                size_t end = LibPath.find_first_of(';', start);

                if (end == std::string::npos)
                {
                    tmp = LibPath.substr(start, LibPath.length() - start);
                }
                else
                {
                    tmp = LibPath.substr(start, end - start);
                }

                if (!tmp.empty())
                {
                    hostLib = OpenLibrary_Helper(tmp.c_str());

                    if (!hostLib)
                    {
                        // Remove the last character in case a new line character snuck in
                        tmp = tmp.substr(0, tmp.size() - 1);
                        hostLib = OpenLibrary_Helper(tmp.c_str());
                    }
                }

                if (end == std::string::npos)
                {
                    break;
                }

                start = end + 1;
            }
            if (!hostLib)
            {
                const char* err = dlerror();
                if (err)
                {
                    FKLog("Could not load host library '%s'", pszPath);
                    FKLog("dlopen failed with error '%s'", err);
                }
            }
            FKLog("Host library '%s' loaded", pszPath);

            return hostLib;
        }
#elif PLATFORM_ANDROID // #elif PLATFORM_IOS
        inline lib::LIBTYPE OpenLibrary(const char* pszPath)
        {
            lib::LIBTYPE hostLib = dlopen(pszPath, RTLD_LAZY | RTLD_GLOBAL);
            if (!hostLib)
            {
                const char* err = dlerror();
                if (err)
                {
                    FKLog("Could not load host library '%s'", pszPath);
                    FKLog("dlopen failed with error '%s'", err);
                }
            }
            FKLog("Host library '%s' loaded", pszPath);
            return hostLib;
        }
#endif  // #elif PLATFORM_ANDROID

        inline void CloseLibrary(lib::LIBTYPE hostLib)
        {
            dlclose(hostLib);
        }

        inline void* GetLibraryFunction(lib::LIBTYPE hostLib, const char* pszName)
        {
            if (hostLib)
            {
                void* func = dlsym(hostLib, pszName);
                return func;
            }
            return nullptr;
        }
#endif // #else if not PLATFORM_WINDOWS
    } // namespace detail

    static lib::LIBTYPE hostLib = nullptr;
    static std::string libraryName;

    static inline lib::LIBTYPE OpenLib(const std::string& libName)
    {
        libraryName = libName;
        hostLib = detail::OpenLibrary(libName.c_str());
        return hostLib;
    }

    static inline void CloseLib(lib::LIBTYPE lib)
    {
        detail::CloseLibrary(lib);
    }

    template<typename PtrType_>
    PtrType_ inline GetLibFunction(lib::LIBTYPE hostLib, const std::string& functionName)
    {
        return reinterpret_cast<PtrType_>(detail::GetLibraryFunction(hostLib, functionName.c_str()));
    }

    template<typename PtrType_>
    PtrType_ inline GetLibFunctionChecked(lib::LIBTYPE hostLib, const std::string& functionName)
    {
        PtrType_ func = GetLibFunction<PtrType_>(hostLib, functionName);
        if (!func)
        {
            FKLog("Failed to load function [%s] from library '%s'.\n", functionName.c_str(), libraryName.c_str());
        }
        return func;
    }

}//namespace lib

NS_FK_END

#endif // END OF FOUNDATIONKIT_OPENLIBRARY_HPP
