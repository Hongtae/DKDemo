//
//  File: DKMemory.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKMemoryHeap-Alloc/Realloc/Free 함수는 기본 malloc,realloc,free 와 같다
//
// DKMemoryVirtual-Alloc/Realloc/Free 함수는 DKMemoryVMPageSize 단위로만 생성한다.
// 시스템의 페이지에 할당되며, 시스템이 관리하는 페이지의 빈 공간보다 크면 할당 되지 않는다.
//
// DKMemoryFile-Alloc/Realloc/Free 함수는 실제 파일을 이용해서 저장공간을 만든다.
// 하드디스크 공간이 부족하면 할당 실패할 수 있다.
// 주의: 시스템 가용메모리보다 큰 크기를 할당하면, 시스템이 멈출 수 있다.
//
// DKMemoryReserved-Alloc/Realloc/Free 함수는 디버깅용
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	// heap 메모리
	DKLIB_API void* DKMemoryHeapAlloc(size_t);
	DKLIB_API void* DKMemoryHeapRealloc(void*, size_t);
	DKLIB_API void  DKMemoryHeapFree(void*);

	// virtual-address 영역 메모리
	// commit, decommit 가능함, decommit 하면 데이터가 소멸된다.
	DKLIB_API void* DKMemoryVirtualAlloc(size_t);
	DKLIB_API void* DKMemoryVirtualRealloc(void*, size_t);
	DKLIB_API void  DKMemoryVirtualFree(void*);

	// 하드디스크의 파일을 이용한 메모리 할당
	DKLIB_API void* DKMemoryFileAlloc(size_t);
	DKLIB_API void* DKMemoryFileRealloc(void*, size_t);
	DKLIB_API void  DKMemoryFileFree(void*);

	// 페이지 관련
	DKLIB_API size_t DKMemoryPageSize(void);	// 기본 할당되는 단위.
	DKLIB_API void* DKMemoryPageReserve(void*, size_t);
	DKLIB_API void DKMemoryPageRelease(void*);
	DKLIB_API void DKMemoryPageCommit(void*, size_t);
	DKLIB_API void DKMemoryPageDecommit(void*, size_t);

	// Don't use below DKMemoryReserved.. functions. (사용하지 말것), DEBUGGING ONLY
	DKLIB_API void* DKMemoryReservedAlloc(size_t);
	DKLIB_API void* DKMemoryReservedRealloc(void*, size_t);
	DKLIB_API void  DKMemoryReservedFree(void*);

	enum DKMemoryLocation
	{
		DKMemoryLocationCustom = 0,
		DKMemoryLocationHeap,
		DKMemoryLocationVirtual,
		DKMemoryLocationFile,
		DKMemoryLocationReserved,
	};

	// 템플릿 클래스를 위한 타입들
	struct DKMemoryHMAllocator
	{
		enum {location = DKMemoryLocationHeap};
		static void* Alloc(size_t s)			{return DKMemoryHeapAlloc(s);}
		static void* Realloc(void* p, size_t s)	{return DKMemoryHeapRealloc(p, s);}
		static void Free(void* p)				{DKMemoryHeapFree(p);}
	};
	class DKMemoryVMAllocator
	{
		enum {location = DKMemoryLocationVirtual};
		static void* Alloc(size_t s)			{return DKMemoryVirtualAlloc(s);}
		static void* Realloc(void* p, size_t s)	{return DKMemoryVirtualRealloc(p, s);}
		static void Free(void* p)				{DKMemoryVirtualFree(p);}
	};
	class DKMemoryFMAllocator
	{
		enum {location = DKMemoryLocationFile};
		static void* Alloc(size_t s)			{return DKMemoryFileAlloc(s);}
		static void* Realloc(void* p, size_t s)	{return DKMemoryFileRealloc(p, s);}
		static void Free(void* p)				{DKMemoryFileFree(p);}
	};	
	class DKMemoryRVAllocator
	{
		enum {location = DKMemoryLocationReserved};
		static void* Alloc(size_t s)			{return DKMemoryReservedAlloc(s);}
		static void* Realloc(void* p, size_t s)	{return DKMemoryReservedRealloc(p, s);}
		static void Free(void* p)				{DKMemoryReservedFree(p);}
	};

	using DKMemoryDefaultAllocator = DKMemoryHMAllocator;
}
