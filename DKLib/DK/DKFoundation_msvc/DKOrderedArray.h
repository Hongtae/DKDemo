//
//  File: DKOrderedArray.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKInclude.h"
#include "DKTypeTraits.h"
#include "DKDummyLock.h"
#include "DKCriticalSection.h"
#include "DKMemory.h"
#include "DKArray.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKOrderedArray
//
// 기본적인 배열 객체
//
// 값은 항상 순서대로 정렬되서 들어간다.
// 해당 값에 대한 빠른 검색이 가능 (Find 함수)
//
// 생성자에서 값 비교함수 필요함.
// (DKArraySortAscending<VALUE> 또는 DKArraySortDescending 사용가능)
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	template <typename VALUE, typename LOCK = DKDummyLock, typename ALLOC = DKMemoryDefaultAllocator>
	class DKOrderedArray
	{
	public:
		typedef LOCK						Lock;
		typedef ALLOC						Allocator;
		typedef DKArray<VALUE, DKDummyLock, Allocator>	Container;
		typedef typename Container::Index	Index;
		typedef DKCriticalSection<LOCK>		CriticalSection;
		typedef DKTypeTraits<VALUE>			ValueTraits;

		typedef bool (*OrderFunc)(const VALUE& lhs, const VALUE& rhs);	// 오름차순은 return lhs < rhs
		typedef bool (*EqualFunc)(const VALUE& lhs, const VALUE& rhs);	// 같을때만 return true

		static const Index invalidIndex = (Index)-1;
		LOCK	lock;		// 외부에서 락을 걸수 있도록 함. (락을 건 상태에서는 형변환과 CountNoLock()만 사용가능)

		// range-based for loop 용 begin, end 멤버함수
		// 주의: range-based 루프를 사용할때 값을 참조하는 동안은 락이 걸리지 않음.
		// 값은 무조건 const VALUE& 로만 사용할 수 있다. (read-only)
		typedef DKArrayRBIterator<DKOrderedArray, const VALUE&>			RBIterator;
		typedef DKArrayRBIterator<const DKOrderedArray, const VALUE&>	ConstRBIterator;
		RBIterator begin(void)				{return RBIterator(*this, 0);}
		ConstRBIterator begin(void) const	{return ConstRBIterator(*this, 0);}
		RBIterator end(void)				{return RBIterator(*this, this->Count());}
		ConstRBIterator end(void) const		{return ConstRBIterator(*this, this->Count());}

		DKOrderedArray(OrderFunc func)
			: orderFunc(func)
		{
			DKASSERT_DEBUG(orderFunc);
		}
		DKOrderedArray(DKOrderedArray&& v)
			: orderFunc(v.orderFunc)
			, container(static_cast<Container&&>(v.container))
		{
			DKASSERT_DEBUG(orderFunc);
		}
		DKOrderedArray(const DKOrderedArray& v)
			: orderFunc(v.orderFunc)
		{
			DKASSERT_DEBUG(orderFunc);
			Insert(v);
		}
		DKOrderedArray(const DKOrderedArray& v, OrderFunc func)
			: orderFunc(func)
		{
			DKASSERT_DEBUG(orderFunc);
			Insert(v);
		}
		DKOrderedArray(const VALUE* v, size_t c, OrderFunc func)
			: orderFunc(func)
		{
			DKASSERT_DEBUG(orderFunc);
			Insert(v, c);
		}
		DKOrderedArray(const VALUE& v, size_t c, OrderFunc func)
			: orderFunc(func)
			, container(v, c)
		{
			DKASSERT_DEBUG(orderFunc);
		}
		DKOrderedArray(std::initializer_list<VALUE> il, OrderFunc func)
			: orderFunc(func)
			, container(il)
		{
			DKASSERT_DEBUG(orderFunc);
		}
		~DKOrderedArray(void)
		{
		}
		DKOrderedArray& operator = (DKOrderedArray&& v)
		{
			if (this != &v)
			{
				CriticalSection guard(lock);
				orderFunc = v.orderFunc;
				container = static_cast<Container&&>(v.container);
			}
			return *this;
		}
		DKOrderedArray& operator = (const DKOrderedArray& v)
		{
			if (this != &v)
			{
				CriticalSection guard(lock);
				orderFunc = v.orderFunc;
				container = v.container;
			}
			return *this;
		}
		bool IsEmpty(void) const
		{
			CriticalSection guard(lock);
			return container.IsEmpty();
		}
		template <typename T> size_t Insert(const DKOrderedArray<VALUE, T>& value)
		{
			typename DKOrderedArray<VALUE, T>::CriticalSection guard(value.lock);
			return Insert((const VALUE*)value, value.Count());
		}
		template <typename T> size_t Insert(const DKArray<VALUE, T>& value)
		{
			typename DKArray<VALUE, T>::CriticalSection guard(value.lock);
			return Insert((const VALUE*)value, value.Count());
		}
		size_t Insert(const VALUE& value)
		{
			CriticalSection guard(lock);
			return InsertContainer(value, 1);
		}
		size_t Insert(const VALUE& value, size_t s)
		{
			CriticalSection guard(lock);
			return InsertContainer(value, s);
		}
		size_t Insert(const VALUE* value, size_t s)
		{
			CriticalSection guard(lock);
			container.Reserve(container.Count() + s);
			for (size_t i = 0; i < s; i++)
				InsertContainer(value[i], 1);
			return container.Count();
		}
		size_t Insert(std::initializer_list<VALUE> il)
		{
			CriticalSection guard(lock);
			container.Reserve(container.Count() + il.size());
			for (const VALUE& v : il)
				InsertContainer(v, 1);
			return container.Count();
		}
		size_t Remove(Index pos)
		{
			CriticalSection guard(lock);
			return container.Remove(pos);
		}
		size_t Remove(Index pos, size_t c)
		{
			CriticalSection guard(lock);
			return container.Remove(pos, c);
		}
		void Clear(void)
		{
			CriticalSection guard(lock);
			container.Clear();
		}
		size_t Count(void) const
		{
			CriticalSection guard(lock);
			return container.Count();
		}
		size_t CountNoLock(void) const		// 외부에서 락을 걸었을때 사용함
		{
			return container.Count();
		}
		void Reserve(size_t c)
		{
			CriticalSection guard(lock);
			container.Reserve(c);
		}
		const VALUE& Value(Index index) const
		{
			CriticalSection guard(lock);
			return container.Value(index);
		}
		operator const VALUE* (void) const	// 외부에서 락을 걸었을때 값을 직접 억세스 할 수 있도록 함.
		{
			return (const VALUE*)container;
		}
		// EnumerateForward / EnumerateBackword: 모든 데이터 열거함수, 이 함수내에서는 배열객체에 값을 추가하거나 제거할 수 없다!! (read-only)
		// lambda enumerator (VALUE&) 또는 (VALUE&, bool*) 형식의 함수객체
		template <typename T> void EnumerateForward(T&& enumerator)
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum { ValidatePType1 = Func::template CanInvokeWithParameterTypes<VALUE&>::Result };
			enum { ValidatePType2 = Func::template CanInvokeWithParameterTypes<VALUE&, bool*>::Result };
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (VALUE&) or (VALUE&,bool*)");
			
			CriticalSection guard(lock);
			container.EnumerateForward(std::forward<T>(enumerator));
		}
		template <typename T> void EnumerateBackward(T&& enumerator)
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum { ValidatePType1 = Func::template CanInvokeWithParameterTypes<VALUE&>::Result };
			enum { ValidatePType2 = Func::template CanInvokeWithParameterTypes<VALUE&, bool*>::Result };
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (VALUE&) or (VALUE&,bool*)");
			
			CriticalSection guard(lock);
			container.EnumerateBackward(std::forward<T>(enumerator));
		}
		// lambda enumerator (const VALUE&) 또는 (const VALUE&, bool*) 형식의 함수객체
		template <typename T> void EnumerateForward(T&& enumerator) const
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum { ValidatePType1 = Func::template CanInvokeWithParameterTypes<const VALUE&>::Result };
			enum { ValidatePType2 = Func::template CanInvokeWithParameterTypes<const VALUE&, bool*>::Result };
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (const VALUE&) or (const VALUE&,bool*)");

			CriticalSection guard(lock);
			container.EnumerateForward(std::forward<T>(enumerator));
		}
		template <typename T> void EnumerateBackward(T&& enumerator) const
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum { ValidatePType1 = Func::template CanInvokeWithParameterTypes<const VALUE&>::Result };
			enum { ValidatePType2 = Func::template CanInvokeWithParameterTypes<const VALUE&, bool*>::Result };
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (const VALUE&) or (const VALUE&,bool*)");

			CriticalSection guard(lock);
			container.EnumerateBackward(std::forward<T>(enumerator));
		}
		Index Find(const VALUE& value, EqualFunc equalFunc = DKArrayCompareEqual<VALUE>) const
		{
			CriticalSection guard(lock);
			return FindNoLock(value, equalFunc);
		}
		Index FindNoLock(const VALUE& value, EqualFunc equalFunc = DKArrayCompareEqual<VALUE>) const
		{
			Index index = 0;
			size_t count = container.Count();
			while (count > 2)
			{
				Index middle = index + count /2;
				if (equalFunc(value, container.Value(middle)))
					return middle;
				if (orderFunc(value, container.Value(middle)))
				{
					count = middle - index;
				}
				else
				{
					Index right = middle+1;
					count -= right - index;
					index = right;
				}
			}
			for (size_t i = 0; i < count; ++i)
			{
				if (equalFunc(value, container.Value(index)))
					return index;
				++index;
			}
			return invalidIndex;
		}
		// 근사값 찾음. (greater 가 true 이면 value 와 같거나 큰값의 인덱스, false 면 value 와 같거나 작은 값의 인덱스)
		Index FindApprox(const VALUE& value, bool greater) const
		{
			CriticalSection guard(lock);
			return FindApproxNoLock(value, greater);
		}
		Index FindApproxNoLock(const VALUE& value, bool greater) const
		{
			Index begin = 0;
			size_t count = container.Count();

			while (count > 2)
			{
				Index middle = begin + count / 2;
				if (orderFunc(value, container.Value(middle)))		// value 가 middle 보다 더 작음
				{
					count = middle - begin + 1;
				}
				else
				{
					count = begin + count - middle;
					begin = middle;
				}
			}

			if (greater)
			{
				for (size_t i = 0; i < count; ++i)
				{
					if (orderFunc(container.Value(begin), value) == false)
						return begin;
					++begin;
				}
			}
			else
			{
				while (count > 0)
				{
					count--;
					if (orderFunc(value, container.Value(begin+count)) == false)
						return begin+count;
				}
			}
			return invalidIndex;
		}
	private:
		// InsertContainer 값 하나를 c 만큼 넣는다.
		size_t InsertContainer(const VALUE& value, size_t c)
		{
			Index index = 0;
			size_t count = container.Count();
			while (count > 2)
			{
				Index middle = index + count /2;
				if (orderFunc(value, container.Value(middle)))
				{
					count = middle - index;
				}
				else
				{
					Index right = middle+1;
					count -= right - index;
					index = right;
				}
			}
			for (size_t i = 0; i < count; i++)
			{
				if (orderFunc(value, container.Value(index)))
					break;
				index++;
			}
			container.Insert(value, c, index);
			return container.Count();
		}

		OrderFunc	orderFunc;
		Container	container;
	};
}