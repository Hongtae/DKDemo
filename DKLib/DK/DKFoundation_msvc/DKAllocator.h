//
//  File: DKAllocator.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKinclude.h"
#include "DKMemory.h"

////////////////////////////////////////////////////////////////////////////////
// DKAllocator
//
// 메모리를 할당하고 소거하는 역활을 한다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKAllocator
	{
	public:
		virtual ~DKAllocator(void);
		virtual void* Alloc(size_t) = 0;
		virtual void Dealloc(void*) = 0;
		virtual DKMemoryLocation Location(void) const = 0;

		static DKAllocator& DefaultAllocator(DKMemoryLocation loc = DKMemoryLocationHeap);

	protected:
		DKAllocator(void);

	private:
		DKAllocator(const DKAllocator&);
		DKAllocator& operator = (const DKAllocator&);
	};
}

////////////////////////////////////////////////////////////////////////////////
// operator new, delete
//
// DKAllocator 객체를 이용하여 메모리를 할당하고, 참조관리를 한다.
// DKObject 클래스로만 사용해야 한다.
////////////////////////////////////////////////////////////////////////////////

DKLIB_API void* operator new (size_t, DKFoundation::DKAllocator&);	// 내부 참조카운터 객체 생성용
DKLIB_API void operator delete (void*, DKFoundation::DKAllocator&);	// 객체 생성시 예외처리용 (예외 발생하면 자동으로 호출됨)
