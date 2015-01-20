//
//  File: DKAllocator.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKMemory.h"
#include "DKAllocator.h"

////////////////////////////////////////////////////////////////////////////////
// DKObjectRef
//
// 특정 객체(포인터)에 대한 참조카운터를 관리한다.
//
// 객체의 포인터와 참조-id (refId, RefIdValue) 를 사용하면, 참조관리를 하지 않더라도 객체의 존재유무를 파악할 수 있다.
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	struct DKLIB_API DKObjectRefCounter
	{
		typedef uintptr_t RefCountValue;
		typedef uint64_t RefIdValue;

		static bool IncrementRefCount(void*, RefIdValue);		// 포인터와 RefIdValue(참조 ID) 모두 같아야 함 (weak-ref 시 사용)
		static bool IncrementRefCount(void*);					// 참조관리 되는 객체가 아닐경우 false 리턴함.
		static bool DecrementRefCount(void*);					// 참조관리 되는 객체가 아닐경우 false 리턴함.
		static bool DecrementRefCountAndUnsetIfEqual(void*, RefCountValue, DKAllocator**);	// 참조값을 감소하고 특정 참조값이면 제거함. 제거되었을때만 true 리턴함
		static bool DecrementRefCountAndUnsetIfZero(void*, DKAllocator**);					// 참조값을 감소하고 0 이 되었다면 제거함. 제거되었을때 true 리턴함

		static bool SetRefCounter(void*, DKAllocator*, RefCountValue, RefIdValue*);	// 참조관리 시작함
		static bool UnsetRefCounterIfEqual(void*, RefCountValue, DKAllocator**);	// 참조 카운트가 특정 값이면 제거함
		static bool UnsetRefCounterIfZero(void*, DKAllocator**);					// 참조 카운트가 0 이면 제거함
		static bool UnsetRefCounter(void*, RefCountValue*, DKAllocator**);			// 무조건 제거함 (제거했을 당시의 참조값을 가져올 수 있음)

		static bool RefCount(void*, RefCountValue*);		// 참조 카운터 리턴
		static bool RefId(void*, RefIdValue*);				// 참조 ID (매우 낮은 확률로 중복될 수 있다)
		static DKMemoryLocation Location(void*);
		static DKAllocator* Allocator(void*);				// 참조 관리되는 객체일 경우 allocator 리턴

		static size_t TableSize(void);				// 디버깅용.
		static void TableDump(size_t*);				// 디버깅용.
	};
}
