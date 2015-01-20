//
//  File: DKStack.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKTypeTraits.h"
#include "DKDummyLock.h"
#include "DKCriticalSection.h"
#include "DKQueue.h"
#include "DKMemory.h"

namespace DKFoundation
{
	template <typename VALUE, typename LOCK = DKDummyLock, typename ALLOC = DKMemoryDefaultAllocator>
	class DKStack
	{
	public:
		typedef ALLOC					Allocator;
		typedef DKQueue<VALUE, LOCK, ALLOC>	Container;

		DKStack(void) {}
		~DKStack(void) {}
		
		void Push(const VALUE& v)		{container.PushFront(v);}
		void Pop(VALUE& v)				{container.PopFront(v);}
		void Pop(void)					{container.PopFront();}

		VALUE& Top(void)				{return container.Front();}
		const VALUE& Top(void) const	{return container.Front();}
		
		bool IsEmpty(void) const		{return container.IsEmpty();}
		void Clear(void)				{container.Clear();}
		
		size_t Count(void)				{return container.Count();}

		// EnumerateForward / EnumerateBackword: 모든 데이터 열거함수, 이 함수내에서는 배열객체에 값을 추가하거나 제거할 수 없다!! (read-only)
		// lambda enumerator (VALUE&) 또는 (VALUE&, bool*) 형식의 함수객체
		template <typename T> void EnumerateForward(T&& enumerator)
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum { ValidatePType1 = Func::template CanInvokeWithParameterTypes<VALUE&>::Result };
			enum { ValidatePType2 = Func::template CanInvokeWithParameterTypes<VALUE&, bool*>::Result };
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (VALUE&) or (VALUE&,bool*)");

			container.EnumerateForward(static_cast<T&&>(enumerator));
		}
		template <typename T> void EnumerateBackward(T&& enumerator)
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum { ValidatePType1 = Func::template CanInvokeWithParameterTypes<VALUE&>::Result };
			enum { ValidatePType2 = Func::template CanInvokeWithParameterTypes<VALUE&, bool*>::Result };
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (VALUE&) or (VALUE&,bool*)");

			container.EnumerateBackward(static_cast<T&&>(enumerator));
		}
		// lambda enumerator (const VALUE&) 또는 (const VALUE&, bool*) 형식의 함수객체
		template <typename T> void EnumerateForward(T&& enumerator) const
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum { ValidatePType1 = Func::template CanInvokeWithParameterTypes<const VALUE&>::Result };
			enum { ValidatePType2 = Func::template CanInvokeWithParameterTypes<const VALUE&, bool*>::Result };
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (const VALUE&) or (const VALUE&,bool*)");

			container.EnumerateForward(static_cast<T&&>(enumerator));
		}
		template <typename T> void EnumerateBackward(T&& enumerator) const
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum { ValidatePType1 = Func::template CanInvokeWithParameterTypes<const VALUE&>::Result };
			enum { ValidatePType2 = Func::template CanInvokeWithParameterTypes<const VALUE&, bool*>::Result };
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (const VALUE&) or (const VALUE&,bool*)");

			container.EnumerateBackward(static_cast<T&&>(enumerator));
		}
	private:
		Container container;
	};
}
