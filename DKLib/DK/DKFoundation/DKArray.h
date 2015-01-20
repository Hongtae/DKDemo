//
//  File: DKArray.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include <initializer_list>
#include "DKTypeTraits.h"
#include "DKDummyLock.h"
#include "DKCriticalSection.h"
#include "DKMemory.h"
#include "DKFunction.h"
#include "DKStaticArray.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKArray
//
// 기본적인 배열 객체
//
// 기본적으로 데이터 추가와 제거에는 락을 건다. 하지만 값을 직접 수정하려면
// 외부에서 따로 락을 걸어야 한다.
//
// 예:
//	{
//		typename MyArrayType::CriticalSection section(array.lock);	// lock with critical-section
//		array[x] = .... // do something with array[]
//	}	// auto-unlock by critical-section end
//
//  VALUE 가 같지만 LOCK 이 다른 경우 Add, Insert 만 가능하다.
//
// 객체의 포인터나 레퍼런스를 가져오는 함수는 thread-safe 하지 않음.
// 값의 추가 제거 또는 값을 복사하는 CopyValue는 thread-safe 함.
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	template <typename T>
	bool DKArraySortAscending(const T& lhs, const T& rhs)		// 오름차순, 작은게 앞
	{
		return lhs < rhs;
	}
	template <typename T>
	bool DKArraySortDescending(const T& lhs, const T& rhs)		// 내림차순, 큰게 앞
	{
		return lhs > rhs;
	}
	template <typename T>
	bool DKArrayCompareEqual(const T& lhs, const T& rhs)		// equal 비교
	{
		return lhs == rhs;
	}

	template <typename VALUE, typename LOCK = DKDummyLock, typename ALLOC = DKMemoryDefaultAllocator>
	class DKArray
	{
		enum {InitialSize = 4,};
	public:
		typedef LOCK					Lock;
		typedef DKCriticalSection<Lock>	CriticalSection;
		typedef size_t					Index;
		typedef DKTypeTraits<VALUE>		ValueTraits;
		typedef ALLOC					Allocator;

		static const Index invalidIndex = (size_t)-1;
		Lock	lock;			// 외부에서 락을 걸수 있도록 한다. 값을 직접 수정할때 사용 (VALUE* 형변환과 CountNoLock() 만 사용 가능)

		// range-based for loop 용 begin, end 멤버함수
		// 주의: range-based 루프를 사용할때 값을 참조하는 동안은 락이 걸리지 않음.
		typedef DKArrayRBIterator<DKArray, VALUE&>				RBIterator;
		typedef DKArrayRBIterator<const DKArray, const VALUE&>	ConstRBIterator;
		RBIterator begin(void)				{return RBIterator(*this, 0);}
		ConstRBIterator begin(void) const	{return ConstRBIterator(*this, 0);}
		RBIterator end(void)				{return RBIterator(*this, this->Count());}
		ConstRBIterator end(void) const		{return ConstRBIterator(*this, this->Count());}

		DKArray(void)
			: data(NULL), count(0), capacity(0)
		{
		}
		DKArray(const VALUE* v, size_t c)
			: data(NULL), count(0), capacity(0)
		{
			Add(v, c);
		}
		DKArray(const VALUE& v, size_t c)
			: data(NULL), count(0), capacity(0)
		{
			Add(v, c);
		}
		DKArray(DKArray&& v)
			: data(NULL), count(0), capacity(0)
		{
			data = v.data;
			count = v.count;
			capacity = v.capacity;
			v.data = NULL;
			v.count = 0;
			v.capacity = 0;
		}
		DKArray(const DKArray& v)
			: data(NULL), count(0), capacity(0)
		{
			Add(v);
		}
		DKArray(std::initializer_list<VALUE> il)
			: data(NULL), count(0), capacity(0)
		{
			if (il.size() > 0)
			{
				ReserveItemCapsNL(il.size());
				for (const VALUE& v : il)
				{
					::new(&data[count]) VALUE(v);
					count++;
				}
			}
		}
		~DKArray(void)
		{
			Clear();

			if (data)
				Allocator::Free(data);
		}
		bool IsEmpty(void) const
		{
			CriticalSection guard(lock);
			return count == 0;
		}
		// 배열의 끝에 배열 추가
		template <typename T> Index Add(const DKArray<VALUE, T>& value)
		{
			typename DKArray<VALUE, T>::CriticalSection guard(value.lock);
			return Add((const VALUE*)value, value.Count());
		}
		// 배열의 끝에 한개의 value 추가
		Index Add(const VALUE& value)
		{
			CriticalSection guard(lock);
			ReserveItemCapsNL(1);
			::new(&data[count]) VALUE(value);
			return count++;
		}
		// 배열의 끝에 s 개의 value 추가
		Index Add(const VALUE* value, size_t s)
		{
			CriticalSection guard(lock);
			ReserveItemCapsNL(s);
			for (Index i = 0; i < s; i++)
				::new(&data[count+i]) VALUE(value[i]);
			count += s;
			return count - s;
		}
		// 배열의 끝에 value 를 s 개 추가
		Index Add(const VALUE& value, size_t s)
		{
			CriticalSection guard(lock);
			ReserveItemCapsNL(s);
			for (Index i = 0; i < s; i++)
				::new(&data[count+i]) VALUE(value);
			count += s;
			return count - s;
		}
		// 배열의 끝에 initializer-list 추가
		Index Add(std::initializer_list<VALUE> il)
		{
			size_t s = il.size();
			CriticalSection guard(lock);
			ReserveItemCapsNL(s);
			for (const VALUE& v : il)
			{
				::new(&data[count]) VALUE(v);
				count++;
			}
			return count - s;
		}
		// 배열의 pos 에 배열 추가
		template <typename T> Index Insert(const DKArray<VALUE, T>& value, Index pos)
		{
			typename DKArray<VALUE, T>::CriticalSection guard(value.lock);
			return Insert((const VALUE*)value, pos);
		}
		// 배열의 pos 에 한개의 value 추가
		Index Insert(const VALUE& value, Index pos)
		{
			CriticalSection guard(lock);
			ReserveItemCapsNL(1);
			if (pos > count)
				pos = count;
			if (pos < count)
				memmove((void*)&data[pos+1], (void*)&data[pos], sizeof(VALUE) * (count - pos));
			::new(&data[pos]) VALUE(value);
			count++;
			return pos;
		}
		// 배열의 pos 에 s 개의 value 추가
		Index Insert(const VALUE* value, size_t s, Index pos)
		{
			CriticalSection guard(lock);
			ReserveItemCapsNL(s);
			if (pos > count)
				pos = count;
			if (pos < count)
				memmove((void*)&data[pos+s], (void*)&data[pos], sizeof(VALUE) * (count - pos));
			for (Index i = 0; i < s; i++)
				::new(&data[pos+i]) VALUE(value[i]);
			count += s;
			return pos;
		}
		// 배열의 pos 에 value 를 s 개 추가
		Index Insert(const VALUE& value, size_t s, Index pos)
		{
			CriticalSection guard(lock);
			ReserveItemCapsNL(s);
			if (pos > count)
				pos = count;
			if (pos < count)
				memmove((void*)&data[pos+s], (void*)&data[pos], sizeof(VALUE) * (count - pos));
			for (Index i = 0; i < s; i++)
				::new(&data[pos+i]) VALUE(value);
			count += s;
			return pos;
		}
		// 배열의 pos 에 initializer_list 추가
		Index Insert(std::initializer_list<VALUE> il, Index pos)
		{
			size_t s = il.size();
			CriticalSection guard(lock);
			ReserveItemCapsNL(s);
			if (pos > count)
				pos = count;
			if (pos < count)
				memmove((void*)&data[pos+s], (void*)&data[pos], sizeof(VALUE) * (count - pos));
			for (const VALUE& v : il)
			{
				::new(&data[pos]) VALUE(v);
				pos++;
			}
			count += s;
			return pos - s;
		}
		// pos 위치의 객체 제거
		size_t Remove(Index pos)
		{
			CriticalSection guard(lock);
			if (pos < count)
			{
				data[pos].~VALUE();
				if (count - pos > 1)
					memmove((void*)&data[pos], (void*)&data[pos+1], sizeof(VALUE) * (count-pos-1));
				count--;
			}
			return count;
		}
		// pos 위치부터 c 만큼 제거
		size_t Remove(Index pos, size_t c)
		{
			CriticalSection guard(lock);
			if (pos < count)
			{
				Index i = 0;
				for (; i < count - pos && i < c; i++)
					data[pos+i].~VALUE();
				if (i > 0)
					memmove((void*)&data[pos], &data[pos+i], sizeof(VALUE) * (count-pos-i));
				count -= i;
			}
			return count;
		}
		void Clear(void)
		{
			CriticalSection guard(lock);
			for (Index i = 0; i < count; i++)
				data[i].~VALUE();

			count = 0;
		}
		size_t Count(void) const
		{
			CriticalSection guard(lock);
			return count;
		}
		size_t Capacity(void) const
		{
			CriticalSection guard(lock);
			return capacity;
		}
		void Resize(size_t s)
		{
			CriticalSection guard(lock);
			if (count > s)			// 데이터 제거
			{
				for (Index i = s; i < count; i++)
					data[i].~VALUE();
			}
			else if (count < s)		// 데이터 추가
			{
				ReserveNL(s);
				for (Index i = count; i < s; i++)
					::new(&data[i]) VALUE();
			}
			count = s;
		}
		void Resize(size_t s, const VALUE& val)
		{
			CriticalSection guard(lock);
			if (count > s)			// 데이터 제거
			{
				for (Index i = s; i < count; i++)
					data[i].~VALUE();
			}
			else if (count < s)		// 데이터 추가
			{
				ReserveNL(s);
				for (Index i = count; i < s; i++)
					::new(&data[i]) VALUE(val);
			}
			count = s;
		}
		void Reserve(size_t c)
		{
			CriticalSection guard(lock);
			ReserveNL(c);
		}
		bool CopyValue(VALUE& value, Index index) const
		{
			CriticalSection guard(lock);
			if (count > index)
			{
				value = data[index];
				return true;
			}
			return false;
		}
		VALUE& Value(Index index)
		{
			CriticalSection guard(lock);
			DKASSERT_DEBUG(index >= 0);
			DKASSERT_DEBUG(count > index);
			return data[index];
		}
		const VALUE& Value(Index index) const
		{
			CriticalSection guard(lock);
			DKASSERT_DEBUG(index >= 0);
			DKASSERT_DEBUG(count > index);
			return data[index];
		}
		operator VALUE* (void)					// 외부에서 락을 걸었을때 값을 직접 억세스 하기 위해 사용함.
		{
			if (count > 0)
				return data;
			return NULL;
		}
		operator const VALUE* (void) const		// 외부에서 락을 걸었을때 값을 직접 억세스 하기 위해 사용함.
		{
			if (count > 0)
				return data;
			return NULL;
		}
		DKArray& operator = (DKArray&& other)
		{
			if (this != &other)
			{
				Clear();
				if (data)
					Allocator::Free(data);
				data = other.data;
				count = other.count;
				capacity = other.capacity;
				other.data = NULL;
				other.count = 0;
				other.capacity = 0;
			}
			return *this;
		}
		DKArray& operator = (const DKArray& value)
		{
			if (this == &value)
				return *this;

			Clear();
			Add(value);
			return *this;
		}
		DKArray& operator = (std::initializer_list<VALUE> il)
		{
			Clear();
			Add(il);
			return *this;
		}
		DKArray operator + (const VALUE& v) const
		{
			DKArray	ret(*this);
			ret.Add(v);
			return ret;
		}
		DKArray operator + (const DKArray& value) const
		{
			DKArray ret(*this);
			ret.Add(value);
			return ret;
		}
		DKArray operator + (std::initializer_list<VALUE> il) const
		{
			DKArray ret(*this);
			ret.Add(il);
			return ret;
		}
		DKArray& operator += (const VALUE& v) const
		{
			Add(v);
			return *this;
		}
		DKArray& operator += (const DKArray& value)
		{
			Add(value);
			return *this;
		}
		DKArray& operator += (std::initializer_list<VALUE> il)
		{
			Add(il);
			return *this;
		}
		void LeftRotate(size_t n)
		{
			CriticalSection guard(lock);
			if (count > 1)
			{
				n = n % count;
				if (n > 0)
				{
					DKStaticArray<VALUE>(data, count).LeftRotate(n);					
				}
			}
		}
		void RightRotate(size_t n)
		{
			CriticalSection guard(lock);
			if (count > 1)
			{
				n = n % count;
				if (n > 0)
				{
					DKStaticArray<VALUE>(data, count).RightRotate(n);
				}
			}
		}
		bool Swap(Index v1, Index v2)
		{
			CriticalSection guard(lock);
			if (v1 != v2 && v1 < count && v2 < count)
			{
				DKStaticArray<VALUE>(data, count).Swap(v1, v2);
				return true;
			}
			return false;
		}
		void Sort(const DKFunctionSignature<bool (const VALUE&, const VALUE&)>* cmp)
		{
			Sort(0, count, cmp);
		}
		void Sort(Index start, size_t count, const DKFunctionSignature<bool (const VALUE&, const VALUE&)>* cmp)
		{
			CriticalSection guard(lock);
			if (count > 1 && (start + count) <= this->count)
			{
				DKStaticArray<VALUE>(&data[start], count).Sort(cmp);
			}			
		}
		template <typename CompareFunc> void Sort(CompareFunc cmp)
		{
			Sort<CompareFunc>(0, count, cmp);
		}
		template <typename CompareFunc> void Sort(Index start, size_t count, CompareFunc cmp)
		{
			CriticalSection guard(lock);
			if (count > 1 && (start + count) <= this->count)
			{
				DKStaticArray<VALUE>(&data[start], count).template Sort<CompareFunc>(cmp);
			}
		}
		// EnumerateForward / EnumerateBackword: 모든 데이터 열거함수, 이 함수내에서는 배열객체에 값을 추가하거나 제거할 수 없다!! (read-only)
		// lambda enumerator (VALUE&) 또는 (VALUE&, bool*) 형식의 함수객체
		template <typename T> void EnumerateForward(T&& enumerator)
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum {ValidatePType1 = Func::template CanInvokeWithParameterTypes<VALUE&>()};
			enum {ValidatePType2 = Func::template CanInvokeWithParameterTypes<VALUE&, bool*>()};
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (VALUE&) or (VALUE&,bool*)");

			EnumerateForward(std::forward<T>(enumerator), typename Func::ParameterNumber());
		}
		template <typename T> void EnumerateBackward(T&& enumerator)
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum {ValidatePType1 = Func::template CanInvokeWithParameterTypes<VALUE&>()};
			enum {ValidatePType2 = Func::template CanInvokeWithParameterTypes<VALUE&, bool*>()};
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (VALUE&) or (VALUE&,bool*)");

			EnumerateBackward(std::forward<T>(enumerator), typename Func::ParameterNumber());
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
		// lambda enumerator (VALUE&)
		template <typename T> void EnumerateForward(T&& enumerator, DKNumber<1>)
		{
			CriticalSection guard(lock);
			for (Index i = 0; i < count; ++i)
				enumerator(data[i]);
		}
		template <typename T> void EnumerateBackward(T&& enumerator, DKNumber<1>)
		{
			CriticalSection guard(lock);
			for (Index i = 1; i <= count; ++i)
				enumerator(data[count - i]);
		}
		// lambda enumerator (const VALUE&)
		template <typename T> void EnumerateForward(T&& enumerator, DKNumber<1>) const
		{
			CriticalSection guard(lock);
			for (Index i = 0; i < count; ++i)
				enumerator(data[i]);
		}
		template <typename T> void EnumerateBackward(T&& enumerator, DKNumber<1>) const
		{
			CriticalSection guard(lock);
			for (Index i = 1; i <= count; ++i)
				enumerator(data[count - i]);
		}
		// lambda enumerator (VALUE&, bool*)
		template <typename T> void EnumerateForward(T&& enumerator, DKNumber<2>)
		{
			CriticalSection guard(lock);
			bool stop = false;
			for (Index i = 0; i < count && !stop; ++i)
				enumerator(data[i], &stop);
		}
		template <typename T> void EnumerateBackward(T&& enumerator, DKNumber<2>)
		{
			CriticalSection guard(lock);
			bool stop = false;
			for (Index i = 1; i <= count && !stop; ++i)
				enumerator(data[count - i], &stop);
		}
		// lambda enumerator (const VALUE&, bool*)
		template <typename T> void EnumerateForward(T&& enumerator, DKNumber<2>) const
		{
			CriticalSection guard(lock);
			bool stop = false;
			for (Index i = 0; i < count && !stop; ++i)
				enumerator(data[i], &stop);
		}
		template <typename T> void EnumerateBackward(T&& enumerator, DKNumber<2>) const
		{
			CriticalSection guard(lock);
			bool stop = false;
			for (Index i = 1; i <= count && !stop; ++i)
				enumerator(data[count - i], &stop);
		}
		void ReserveNL(size_t c)
		{
			if (c <= capacity)
				return;

			if (data)
				data = (VALUE*)Allocator::Realloc(data, sizeof(VALUE) * c);
			else
				data = (VALUE*)Allocator::Alloc(sizeof(VALUE) * c);

			capacity = c;
		}
		void ReserveItemCapsNL(size_t c)
		{
			if (c > 0)
			{
				size_t minimum = c > InitialSize ? c : InitialSize;

				if (capacity < c + count || count == capacity)
				{
					ReserveNL(count + ((count/2) > minimum ? (count/2):minimum));
				}
			}
		}
		VALUE*	data;
		size_t	count;
		size_t	capacity;
	};
}
