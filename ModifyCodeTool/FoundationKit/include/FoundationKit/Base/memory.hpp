/****************************************************************************
  Copyright (c) 2017 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_MEMORY_HPP
#define FOUNDATIONKIT_MEMORY_HPP
#include "FoundationKit/GenericPlatformMacros.hpp"

#if PLATFORM_WINDOWS
#include <malloc.h>
inline size_t MallocUsableSize(void* ptr)
{
	return _msize(ptr);
}
#elif PLATFORM_ANDROID || PLATFORM_LINUX
#include <malloc.h>
#if __ANDROID_API__ < 17
struct chunk {
	size_t psize, csize;
	struct chunk *next, *prev;
};
#define OVERHEAD (2*sizeof(size_t))
#define CHUNK_SIZE(c) ((c)->csize & -2)
#define MEM_TO_CHUNK(p) (struct chunk *)((char *)(p) - OVERHEAD)
#endif

inline size_t MallocUsableSize(void* ptr)
{
#if __ANDROID_API__ < 17
	return ptr ? CHUNK_SIZE(MEM_TO_CHUNK(ptr)) - OVERHEAD : 0;
#else
	return malloc_usable_size(ptr);
#endif
}

#elif PLATFORM_APPLE
#include <malloc/malloc.h>
inline size_t MallocUsableSize(void* ptr)
{
	return malloc_size(ptr);
}
#else
#endif


/// Override new + delete operators (and array versions) in every module.
/// This prevents the possibility of mismatched new/delete calls such as a new[] that
/// uses allocator and a delete[] that uses the system allocator.
#define REPLACEMENT_OPERATOR_NEW_AND_DELETE \
	void* operator new  ( size_t Size                        )              { return FMemory::Malloc( Size ); } \
	void* operator new[]( size_t Size                        )              { return FMemory::Malloc( Size ); } \
	void* operator new  ( size_t Size, const std::nothrow_t& )              { return FMemory::Malloc( Size ); } \
	void* operator new[]( size_t Size, const std::nothrow_t& )              { return FMemory::Malloc( Size ); } \
	void operator delete  ( void* Ptr )                                     { FMemory::Free( Ptr ); } \
	void operator delete[]( void* Ptr )                                     { FMemory::Free( Ptr ); } \
	void operator delete  ( void* Ptr, const std::nothrow_t& )              { FMemory::Free( Ptr ); } \
	void operator delete[]( void* Ptr, const std::nothrow_t& )              { FMemory::Free( Ptr ); } \
	void operator delete  ( void* Ptr, size_t Size )                        { FMemory::Free( Ptr ); } \
	void operator delete[]( void* Ptr, size_t Size )                        { FMemory::Free( Ptr ); } \
	void operator delete  ( void* Ptr, size_t Size, const std::nothrow_t& ) { FMemory::Free( Ptr ); } \
	void operator delete[]( void* Ptr, size_t Size, const std::nothrow_t& ) { FMemory::Free( Ptr ); }

#endif // END OF FOUNDATIONKIT_MEMORY_HPP