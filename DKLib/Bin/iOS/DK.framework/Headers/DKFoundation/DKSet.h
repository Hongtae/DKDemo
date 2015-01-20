//
//  File: DKSet.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include <initializer_list>
#include "../DKInclude.h"
#include "DKAVLTree.h"
#include "DKDummyLock.h"
#include "DKCriticalSection.h"
#include "DKTypeTraits.h"

////////////////////////////////////////////////////////////////////////////////
// DKSet
//
// 밸런싱 트리(기본적으로 DKAVLTree 사용함) 를 이용한 객체 집합.
//
// VALUE: 기본 타입
// LOCK: 억세스 락
// COMPARE: VALUE 간 비교
//
// VALUE 만 같고 LOCK, CMP 가 다를 경우 Union, Intersect 만 가능
////////////////////////////////////////////////////////////////////////////////


namespace DKFoundation
{
	template <typename VALUE> struct DKSetComparison		// T 와 T 의 비교
	{
		int operator () (const VALUE& lhs, const VALUE& rhs) const
		{
			if (lhs > rhs)
				return 1;
			else if (lhs < rhs)
				return -1;
			return 0;
		}
	};

	template <typename VALUE, typename LOCK = DKDummyLock, typename COMPARE = DKSetComparison<VALUE>, typename ALLOC = DKMemoryDefaultAllocator>
	class DKSet
	{
	public:
		typedef VALUE													Value;
		typedef LOCK													Lock;
		typedef COMPARE													Compare;
		typedef ALLOC													Allocator;
		typedef DKAVLTree<Value, Value, Compare, Compare, DKTreeValueCopy<VALUE>, Allocator>	Container;
		typedef DKCriticalSection<Lock>									CriticalSection;
		typedef DKTypeTraits<Value>										ValueTraits;

		Lock	lock;			// 외부에서 락을 걸수 있게 한다. ContainsNoLock(), CountNoLock() 만 사용 가능함.

		DKSet(void)
		{
		}
		DKSet(DKSet&& s)
			: container(static_cast<Container&&>(s.container))
		{
		}
		DKSet(const DKSet& s)		// copy constructor 에서는 같은 형식의 DKSet 만 받는다. (템플릿으로 만들면 사용할수 없게됨)
		{
			CriticalSection guard(s.lock);
			container = s.container;
		}
		DKSet(const Value* v, size_t n)
		{
			for (size_t i = 0; i < n; ++i)
				container.Insert(v[i]);
		}
		DKSet(std::initializer_list<Value> il)
		{
			for (const Value& v : il)
				container.Insert(v);
		}
		~DKSet(void)
		{
		}
		void Insert(const Value& v)
		{
			CriticalSection guard(lock);
			container.Insert(v);
		}
		void Insert(const Value* v, size_t n)
		{
			CriticalSection guard(lock);
			for (size_t i = 0; i < n; ++i)
				container.Insert(v[i]);
		}
		void Insert(std::initializer_list<Value> il)
		{
			CriticalSection guard(lock);
			for (const Value& v : il)
				container.Insert(v);
		}
		template <typename L, typename C, typename A> DKSet& Union(const DKSet<VALUE,L,C,A>& s)
		{
			CriticalSection guard(lock);
			s.EnumerateForward([this](const VALUE& val) { container.Insert(val); });
			return *this;
		}
		template <typename L, typename C, typename A> DKSet& Intersect(const DKSet<VALUE,L,C,A>& s)
		{
			CriticalSection guard(lock);
			s.EnumerateForward([this](const VALUE& val) {this->container.Remove(val);});
			return *this;
		}
		void Remove(const Value& v)
		{
			CriticalSection guard(lock);
			container.Remove(v);
		}
		void Remove(std::initializer_list<Value> il)
		{
			CriticalSection guard(lock);
			container.Remove(il);
		}
		void Clear(void)
		{
			CriticalSection guard(lock);
			container.Clear();
		}
		bool Contains(const Value& v) const
		{
			CriticalSection guard(lock);
			return container.Find(v) != NULL;
		}
		bool ContainsNoLock(const Value& v) const
		{
			return container.Find(v) != NULL;
		}
		bool IsEmpty(void) const
		{
			CriticalSection guard(lock);
			return container.Count() == 0;
		}
		size_t Count(void) const
		{
			CriticalSection guard(lock);
			return container.Count();
		}
		size_t CountNoLock(void) const
		{
			return container.Count();
		}
		DKSet& operator = (DKSet&& s)
		{
			if (this != &s)
			{
				CriticalSection guard(lock);
				container = static_cast<Container&&>(s.container);
			}
			return *this;
		}
		DKSet& operator = (const DKSet& s)
		{
			if (this != &s)
			{
				CriticalSection guardOther(s.lock);
				CriticalSection guardSelf(lock);

				container = s.container;
			}
			return *this;
		}
		DKSet& operator = (std::initializer_list<Value> il)
		{
			CriticalSection guard(lock);
			container.Clear();
			for (const Value& v : il)
				container.Insert(v);
			return *this;
		}
		// lambda enumerator (const VALUE&) 또는 (const VALUE&, bool*) 형식의 함수객체
		template <typename T> void EnumerateForward(T&& enumerator) const
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum {ValidatePType1 = Func::template CanInvokeWithParameterTypes<const VALUE&>()};
			enum {ValidatePType2 = Func::template CanInvokeWithParameterTypes<const VALUE&, bool*>()};
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (const VALUE&) or (const VALUE&,bool*)");

			EnumerateForward(std::forward<T>(enumerator), typename Func::ParameterNumber());
		}
		template <typename T> void EnumerateBackward(T&& enumerator) const
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum {ValidatePType1 = Func::template CanInvokeWithParameterTypes<const VALUE&>()};
			enum {ValidatePType2 = Func::template CanInvokeWithParameterTypes<const VALUE&, bool*>()};
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (const VALUE&) or (const VALUE&,bool*)");

			EnumerateBackward(std::forward<T>(enumerator), typename Func::ParameterNumber());
		}
	private:
		// lambda enumerator (const VALUE&)
		template <typename T> void EnumerateForward(T&& enumerator, DKNumber<1>) const
		{
			CriticalSection guard(lock);
			container.EnumerateForward([&enumerator](const VALUE& val, bool*) {enumerator(val);});
		}
		template <typename T> void EnumerateBackward(T&& enumerator, DKNumber<1>) const
		{
			CriticalSection guard(lock);
			container.EnumerateBackward([&enumerator](const VALUE& val, bool*) {enumerator(val);});
		}
		// lambda enumerator (const VALUE&, bool*)
		template <typename T> void EnumerateForward(T&& enumerator, DKNumber<2>) const
		{
			CriticalSection guard(lock);
			container.EnumerateForward(enumerator);
		}
		template <typename T> void EnumerateBackward(T&& enumerator, DKNumber<2>) const
		{
			CriticalSection guard(lock);
			container.EnumerateBackward(enumerator);
		}

		Container container;
	};
}

