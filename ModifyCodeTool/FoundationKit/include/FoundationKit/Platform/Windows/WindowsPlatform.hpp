/****************************************************************************
  Copyright (c) 2017 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_WINDOWSPLATFORM_HPP
#define FOUNDATIONKIT_WINDOWSPLATFORM_HPP
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(WIN64) || defined(_WIN64)

// WINVER _WIN32_WINNT 
// https://msdn.microsoft.com/en-us/library/6sehtctf.aspx

/**
 * Macro for marking up deprecated code, functions and types.
 *
 * Features that are marked as deprecated are scheduled to be removed from the code base
 * in a future release. If you are using a deprecated feature in your code, you should
 * replace it before upgrading to the next release. See the Upgrade Notes in the release
 * notes for the release in which the feature was marked deprecated.
 *
 * Sample usage (note the slightly different syntax for classes and structures):
 *
 *		DEPRECATED(4.6, "Message")
 *		void Function();
 *
 *		struct DEPRECATED(4.6, "Message") MODULE_API MyStruct
 *		{
 *			// StructImplementation
 *		};
 *		class DEPRECATED(4.6, "Message") MODULE_API MyClass
 *		{
 *			// ClassImplementation
 *		};
 *
 * @param VERSION The release number in which the feature was marked deprecated.
 * @param MESSAGE A message containing upgrade notes.
 */
#define DEPRECATED(VERSION, MESSAGE) __declspec(deprecated(MESSAGE " Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile."))
// Symbol export and import definitions
#define API_EXPORT __declspec(dllexport)
#define API_IMPORT __declspec(dllimport)
// Alignment.
#define ATTRIBUTE_PACK(n)
#define ATTRIBUTE_ALIGN(n) __declspec(align(n)) 
#define ATTRIBUTE_UNUSED   (void)
#define ATTRIBUTE_USED 
#define ATTRIBUTE_NORETURN __declspec(noreturn)	  /* Indicate that the function never returns. */
#define FORCEINLINE __forceinline				  /* Force code to be inline */
#define FORCENOINLINE __declspec(noinline)        /* Force code to NOT be inline */	
#define THREAD_LOCAL __declspec(thread)
#define FILEPATH_MAX MAX_PATH

__pragma (warning(disable:4127))
#pragma warning(disable:4127)
#define _XKEYCHECK_H // disable windows xkeycheck.h

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

//#ifndef NOMINMAX
//#define NOMINMAX
//#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#ifndef _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS 1
#endif

/*
* Include windows.h without Windows Sockets 1.1 to prevent conflicts with
* Windows Sockets 2.0.
*/
#include <windows.h>

#endif // END OF defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(WIN64) || defined(_WIN64)
#endif // END OF FOUNDATIONKIT_WINDOWSPLATFORM_HPP