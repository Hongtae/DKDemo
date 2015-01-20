//
//  File: DKMap.h
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
// DKMap
//
// 밸런싱 트리(기본적으로 DKAVLTree 사용)를 이용한 Key-Value Pair 맵
//
// Insert: 값이 없다면 새로 추가하고 값이 존재한다면 실패
// Update: 값이 없다면 새로 추가하고 있다면 갱신한다.
//
// 쓰레드 동기화: 기본적으로 추가,삭제,검색 에 대해서 락을 사용한다.
// 검색결과에 대한 값을 수정하기 위해서는 사전에 락을 걸어놓아야 한다.
//
// 예:
//	{
//		typename MyMapType::CriticalSection section(map.lock);	// lock with critical-section
//		MyMapType::Pair* p = map.Find(something);
//		.... // do something with p
//	}	// auto-unlock by critical-section end
//
// KEY: 검색용 키
// VALUE: 자료형 (KEY, VALUE 가 한쌍으로 저장된다)
// LOCK: 자료 억세스용 락
// COMPARE: KEY 의 비교를 위한 객체
//
// KEY, VALUE 만 같고 LOCK, COMPARE 가 다를경우, Insert, Update 만 가능
//
// 데이터 열거 방식.
// typedef DKMap<Key,Value> MyMap;
// MyMap map;
// auto enumerator1 = [](const MyMap::Pair& pair) {...}
// auto enumerator2 = [](const MyMap::Pair& pair, bool* stop) {...}
// map.EnumerateForward(enumerator1);
// map.EnumerateForward(enumerator2);	// bool* 으로 중간에 취소 가능함.
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	template <typename KEY, typename VALUE> class DKMapPair
	{
	public:
		DKMapPair(const KEY& k, const VALUE& v)
			: key(k), value(v) {}
		DKMapPair(const DKMapPair& p)
			: key(p.key), value(p.value) {}

		KEY key;
		VALUE value;
	};
	template <typename KEY> struct DKMapKeyComparison
	{
		int operator () (const KEY& lhs, const KEY& rhs) const
		{
			if (lhs > rhs)
				return 1;
			else if (lhs < rhs)
				return -1;
			return 0;
		}
	};
	template <typename VALUE> struct DKMapValueCopy
	{
		void operator () (VALUE& dst, const VALUE& src) const
		{
			dst = src;
		}
	};

	template <
		typename KEY,										// 키
		typename VALUE,										// 값
		typename LOCK = DKDummyLock,						// 락
		typename COMPARE = DKMapKeyComparison<KEY>,			// 키 비교형식
		typename COPY = DKMapValueCopy<VALUE>,				// 값 복사형식
		typename ALLOC = DKMemoryDefaultAllocator			// 메모리 할당
	>
	class DKMap
	{
	public:
		typedef LOCK						Lock;
		typedef COMPARE						Compare;
		typedef COPY						Copy;
		typedef DKMapPair<const KEY, VALUE>	Pair;
		typedef DKCriticalSection<Lock>		CriticalSection;
		typedef ALLOC						Allocator;
		typedef DKTypeTraits<KEY>			KeyTraits;
		typedef DKTypeTraits<VALUE>			ValueTraits;

		struct PairComparison
		{
			int operator () (const Pair& lhs, const Pair& rhs) const
			{
				return cmp(lhs.key, rhs.key);
			}
			Compare cmp;
		};
		struct KeyComparison
		{
			int operator () (const Pair& lhs, const KEY& key) const
			{
				return cmp(lhs.key, key);
			}
			Compare cmp;
		};
		struct PairCopy
		{
			void operator () (Pair& dst, const Pair& src) const
			{
				copy(dst.value, src.value);
			}
			Copy copy;
		};
		typedef DKAVLTree<Pair, KEY, PairComparison, KeyComparison, PairCopy, Allocator> Container;

		Lock	lock;			// 외부에서 락을 걸수 있게 한다. FindNoLock(), CountNoLock() 만 사용가능함

		DKMap(void)
		{
		}
		DKMap(DKMap&& m)
			: container(static_cast<Container&&>(m.container))
		{
		}
		DKMap(const DKMap& m)
		{
			CriticalSection guard(m.lock);
			container = m.container;			
		}
		DKMap(std::initializer_list<Pair> il)
		{
			for (const Pair& p : il)
				container.Insert(p);
		}
		~DKMap(void)
		{
			Clear();
		}
		// Update: 기존 아이템은 갱신하고 없는것은 새로 추가
		void Update(const Pair& p)
		{
			CriticalSection guard(lock);
			container.Update(p);			
		}
		void Update(const KEY& k, const VALUE& v)
		{
			Update(Pair(k,v));
		}
		void Update(const Pair* p, size_t size)
		{
			for (size_t i = 0; i < size; i++)
				Update(p[i]);
		}
		template <typename LK, typename T1, typename T2, typename T3>
		void Update(const DKMap<KEY, VALUE, LK, T1, T2, T3>& m)
		{
			CriticalSection guard(lock);
			m.EnumerateForward([this](const typename DKMap<KEY, VALUE, LK, T1, T2, T3>::Pair& pair)
			{
				container.Update(pair);
			});			
		}
		void Update(std::initializer_list<Pair> il)
		{
			CriticalSection guard(lock);
			for (const Pair& p : il)
				container.Update(p);
		}
		// Insert: 존재하지 않는 아이템만 추가
		bool Insert(const Pair& p)
		{
			CriticalSection guard(lock);
			return container.Insert(p) != NULL;
		}
		bool Insert(const KEY& k, const VALUE& v)
		{
			return Insert(Pair(k, v));
		}
		size_t Insert(const Pair* p, size_t size)
		{
			size_t ret = 0;
			for (size_t i = 0; i < size; i++)
				if (Insert(p[i]))
					ret++;
			return ret;
		}
		template <typename LK, typename T1, typename T2, typename T3>
		size_t Insert(const DKMap<KEY, VALUE, LK, T1, T2, T3>& m)
		{
			size_t n = 0;
			CriticalSection guard(lock);
			m.EnumerateForward([this, &n](const typename DKMap<KEY, VALUE, LK, T1, T2, T3>::Pair& pair)
			{
				if (container.Insert(pair) != NULL)
					n++;
			});
			return n;
		}
		size_t Insert(std::initializer_list<Pair> il)
		{
			size_t n = 0;
			CriticalSection guard(lock);
			for (const Pair& p : il)
			{
				if (container.Insert(p) != NULL)
					n++;
			}
			return n;
		}
		void Remove(const KEY& k)
		{
			CriticalSection guard(lock);
			container.Remove(k);
		}
		void Remove(std::initializer_list<KEY> il)
		{
			CriticalSection guard(lock);
			for (const KEY& k : il)
				container.Remove(k);
		}
		void Clear(void)
		{
			CriticalSection guard(lock);
			container.Clear();
		}
		Pair* Find(const KEY& k)
		{
			return const_cast<Pair*>(static_cast<const DKMap&>(*this).Find(k));
		}
		const Pair* Find(const KEY& k) const
		{
			CriticalSection guard(lock);
			return FindNoLock(k);
		}
		// 락을 걸지 않음 (외부에서 미리 락 걸고 사용, 검색 결과 값을 수정할 때 사용한다.)
		Pair* FindNoLock(const KEY& k)
		{
			return const_cast<Pair*>(static_cast<const DKMap&>(*this).FindNoLock(k));
		}
		const Pair* FindNoLock(const KEY& k) const
		{
			return container.Find(k);
		}
		VALUE& Value(const KEY& k)			// k 값이 없으면 새로 만들어진다.
		{
			CriticalSection guard(lock);
			Pair* p = FindNoLock(k);
			if (p == NULL)
				p = const_cast<Pair*>(container.Insert(Pair(k, VALUE())));
			return p->value;
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
		DKMap& operator = (DKMap&& m)
		{
			if (this != &m)
			{
				CriticalSection guard(lock);
				container = static_cast<Container&&>(m.container);
			}
			return *this;
		}
		DKMap& operator = (const DKMap& m)
		{
			if (this != &m)
			{
				CriticalSection guardOther(m.lock);
				CriticalSection guardSelf(lock);

				container = m.container;
			}
			return *this;
		}
		DKMap& operator = (std::initializer_list<Pair> il)
		{
			CriticalSection guard(lock);
			container.Clear();
			for (const Pair& p : il)
				container.Insert(p);
			return *this;
		}
		// lambda enumerator (VALUE&) 또는 (VALUE&, bool*) 형식의 함수객체
		template <typename T> void EnumerateForward(T&& enumerator)
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum {ValidatePType1 = Func::template CanInvokeWithParameterTypes<Pair&>()};
			enum {ValidatePType2 = Func::template CanInvokeWithParameterTypes<Pair&, bool*>()};
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (VALUE&) or (VALUE&,bool*)");

			EnumerateForward(std::forward<T>(enumerator), typename Func::ParameterNumber());
		}
		template <typename T> void EnumerateBackward(T&& enumerator)
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum {ValidatePType1 = Func::template CanInvokeWithParameterTypes<Pair&>()};
			enum {ValidatePType2 = Func::template CanInvokeWithParameterTypes<Pair&, bool*>()};
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (VALUE&) or (VALUE&,bool*)");

			EnumerateBackward(std::forward<T>(enumerator), typename Func::ParameterNumber());
		}
		// lambda enumerator (const VALUE&) 또는 (const VALUE&, bool*) 형식의 함수객체
		template <typename T> void EnumerateForward(T&& enumerator) const
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum {ValidatePType1 = Func::template CanInvokeWithParameterTypes<const Pair&>()};
			enum {ValidatePType2 = Func::template CanInvokeWithParameterTypes<const Pair&, bool*>()};
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (const VALUE&) or (const VALUE&,bool*)");

			EnumerateForward(std::forward<T>(enumerator), typename Func::ParameterNumber());
		}
		template <typename T> void EnumerateBackward(T&& enumerator) const
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum {ValidatePType1 = Func::template CanInvokeWithParameterTypes<const Pair&>()};
			enum {ValidatePType2 = Func::template CanInvokeWithParameterTypes<const Pair&, bool*>()};
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (const VALUE&) or (const VALUE&,bool*)");

			EnumerateBackward(std::forward<T>(enumerator), typename Func::ParameterNumber());
		}

	private:
		// lambda enumerator (VALUE&)
		template <typename T> void EnumerateForward(T&& enumerator, DKNumber<1>)
		{
			CriticalSection guard(lock);
			container.EnumerateForward([&enumerator](Pair& val, bool*) {enumerator(val);});
		}
		template <typename T> void EnumerateBackward(T&& enumerator, DKNumber<1>)
		{
			CriticalSection guard(lock);
			container.EnumerateBackward([&enumerator](Pair& val, bool*) {enumerator(val);});
		}
		// lambda enumerator (const VALUE&)
		template <typename T> void EnumerateForward(T&& enumerator, DKNumber<1>) const
		{
			CriticalSection guard(lock);
			container.EnumerateForward([&enumerator](const Pair& val, bool*) {enumerator(val);});
		}
		template <typename T> void EnumerateBackward(T&& enumerator, DKNumber<1>) const
		{
			CriticalSection guard(lock);
			container.EnumerateBackward([&enumerator](const Pair& val, bool*) {enumerator(val);});
		}
		// lambda enumerator (VALUE&, bool*)
		template <typename T> void EnumerateForward(T&& enumerator, DKNumber<2>)
		{
			CriticalSection guard(lock);
			container.EnumerateForward(enumerator);
		}
		template <typename T> void EnumerateBackward(T&& enumerator, DKNumber<2>)
		{
			CriticalSection guard(lock);
			container.EnumerateBackward(enumerator);
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
		
		Container	container;
	};
}
