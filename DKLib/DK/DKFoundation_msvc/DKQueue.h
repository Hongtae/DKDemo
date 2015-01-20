//
//  File: DKQueue.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include <initializer_list>
#include "../DKInclude.h"
#include "DKObject.h"
#include "DKDummyLock.h"
#include "DKCriticalSection.h"
#include "DKMemory.h"
#include "DKFunction.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKQueue
//
// 앞뒤로 값을 넣거나 뺄수 있는 큐 (DKArray 보다 복사 오버헤드가 적음)
// 중간에 값을 넣거나 뺄수는 없다.
//
// PushFront() : 넣은 값들의 맨 처음 값의 포인터 리턴 (임시 포인터)
// PushBack() : 넣은 값들의 맨 마지막 값의 포인터 리턴 (임시 포인터)
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	template <typename VALUE, typename LOCK = DKDummyLock, typename ALLOC = DKMemoryDefaultAllocator>
	class DKQueue
	{
		enum {InitialSize = 32,};
	public:
		typedef LOCK						Lock;
		typedef DKCriticalSection<Lock>		CriticalSection;
		typedef ALLOC						Allocator;
		Lock	lock;			// 외부에서 락을 걸수 있게 한다. (const VALUE* 형변환과 CountNoLock() 만 사용 가능)

		DKQueue(void)
			: begin(0)
			, count(0)
			, maxSize(0)
			, data(NULL)
		{
		}
		DKQueue(const VALUE* v, size_t c)
			: begin(0)
			, count(0)
			, maxSize(0)
			, data(NULL)
		{
			PushBack(v, c);
		}
		DKQueue(const VALUE& v, size_t c)
			: begin(0)
			, count(0)
			, maxSize(0)
			, data(NULL)
		{
			PushBack(v, c);
		}
		DKQueue(DKQueue&& queue)
			: begin(0)
			, count(0)
			, maxSize(0)
			, data(NULL)
		{
			begin = queue.begin;
			count = queue.count;
			maxSize = queue.maxSize;
			data = queue.data;
			
			queue.begin = 0;
			queue.count = 0;
			queue.maxSize = 0;
			queue.data = NULL;
		}
		DKQueue(const DKQueue& queue)
			: begin(0)
			, count(0)
			, maxSize(0)
			, data(NULL)
		{
			PushBack(queue);
		}
		~DKQueue(void)
		{
			Clear();
			if (data)
				Allocator::Free(data);
		}
		void Clear(void)
		{
			CriticalSection guard(lock);
			if (data && count)
			{
				for (size_t i = begin; i < begin+count; i++)
				{
					data[i].~VALUE();
				}
			}
			count = 0;
			Balance();
		}
		VALUE* PushFront(const DKQueue& queue)
		{
			CriticalSection guard(queue.lock);
			return PushFront((const VALUE*)queue, queue.count);
		}
		VALUE* PushFront(const VALUE* values, size_t n)
		{
			CriticalSection guard(lock);
			ReserveFront(n);
			for (size_t i = 0; i < n; i++)
			{
				::new(&data[--begin]) VALUE(values[n - i - 1]);
				count++;
			}
			Balance();
			if (count > 0)
				return &data[begin];
			return NULL;
		}
		VALUE* PushFront(const VALUE& value, size_t n)
		{
			CriticalSection guard(lock);
			ReserveFront(n);
			for (size_t i = 0; i < n; i++)
			{
				::new(&data[--begin]) VALUE(value);
				count++;
			}
			Balance();
			if (count > 0)
				return &data[begin];
			return NULL;
		}
		VALUE* PushFront(const VALUE& value)
		{
			return PushFront(value, 1);
		}
		VALUE* PushFront(std::initializer_list<VALUE> il)
		{
			CriticalSection guard(lock);
			ReserveFront(il.size());

			for (const VALUE& v : il)
			{
				::new(&data[--begin]) VALUE(v);
				count++;
			}
			Balance();
			if (count > 0)
				return &data[begin];
			return NULL;
		}
		VALUE* PushBack(const DKQueue& queue)
		{
			CriticalSection	guard(queue.lock);
			return PushBack((const VALUE*)queue, queue.count);
		}
		VALUE* PushBack(const VALUE* values, size_t n)
		{
			CriticalSection guard(lock);
			ReserveBack(n);
			for (size_t i = 0; i < n; i++)
			{
				::new(&data[begin+count]) VALUE(values[i]);
				count++;
			}
			Balance();
			if (count > 0)
				return &data[begin+count-1];
			return NULL;
		}
		VALUE* PushBack(const VALUE& value, size_t n)
		{
			CriticalSection guard(lock);
			ReserveBack(n);
			for (size_t i = 0; i < n; i++)
			{
				::new(&data[begin+count]) VALUE(value);
				count++;
			}
			Balance();
			if (count > 0)
				return &data[begin+count-1];
			return NULL;
		}
		VALUE* PushBack(const VALUE& value)
		{
			return PushBack(value, 1);
		}
		VALUE* PushBack(std::initializer_list<VALUE> il)
		{
			CriticalSection guard(lock);
			ReserveBack(il.size());
			for (const VALUE& v : il)
			{
				::new(&data[begin+count]) VALUE(v);
				count++;
			}
			Balance();
			if (count > 0)
				return &data[begin+count-1];
			return NULL;
		}
		bool IsEmpty(void) const
		{
			CriticalSection guard(lock);
			return count == 0;
		}
		bool PopFront(VALUE& ret)
		{
			CriticalSection guard(lock);

			if (count > 0)
			{
				ret = data[begin];
				data[begin].~VALUE();
				begin++;
				count--;
				Balance();
				return true;
			}
			return false;
		}
		VALUE PopFront(void)
		{
			CriticalSection guard(lock);
			DKASSERT_DEBUG(count > 0);		// 에러!! 큐가 비었음

			VALUE ret = data[begin];
			data[begin].~VALUE();
			begin++;
			count--;
			Balance();
			return ret;
		}
		bool PopBack(VALUE& ret)
		{
			CriticalSection guard(lock);

			if (count > 0)
			{
				ret = data[begin+count-1];
				data[begin+count-1].~VALUE();
				count--;
				Balance();
				return true;
			}
			return false;
		}
		VALUE PopBack(void)
		{
			CriticalSection guard(lock);
			DKASSERT_DEBUG(count > 0);		// 에러!! 큐가 비었음

			VALUE ret = data[begin+count-1];
			data[begin+count-1].~VALUE();
			count--;
			Balance();
			return ret;
		}
		VALUE& Front(void)
		{
			CriticalSection guard(lock);
			DKASSERT_DEBUG(count > 0);		// 에러!! 큐가 비었음
			return data[begin];
		}
		const VALUE& Front(void) const
		{
			CriticalSection guard(lock);
			DKASSERT_DEBUG(count > 0);		// 에러!! 큐가 비었음
			return data[begin];
		}
		bool Front(VALUE& ret) const
		{
			CriticalSection guard(lock);			
			if (count > 0)
			{
				ret = data[begin];
				return true;
			}
			return false;
		}
		VALUE& Back(void)
		{
			CriticalSection guard(lock);
			DKASSERT_DEBUG(count > 0);		// 에러!! 큐가 비었음
			return data[begin+count-1];
		}
		const VALUE& Back(void) const
		{
			CriticalSection guard(lock);
			DKASSERT_DEBUG(count > 0);		// 에러!! 큐가 비었음
			return data[begin+count-1];
		}
		bool Back(VALUE& ret) const
		{
			CriticalSection guard(lock);			
			if (count > 0)
			{
				ret = data[begin+count-1];
				return true;
			}
			return false;
		}
		bool CopyValue(VALUE& value, unsigned long index) const
		{
			CriticalSection guard(lock);
			if (count > index)
			{
				value = data[begin+index];
				return true;
			}
			return false;
		}
		VALUE& Value(unsigned long index)
		{
			CriticalSection guard(lock);
			DKASSERT_DEBUG(count > index);
			return data[begin+index];
		}
		const VALUE& Value(unsigned long index) const
		{
			CriticalSection guard(lock);
			DKASSERT_DEBUG(count > index);
			return data[begin+index];
		}
		operator VALUE* (void)					// 외부에서 락을 걸고 값을 직접 억세스 할때 사용함
		{
			if (count > 0)
				return &data[begin];
			return NULL;
		}
		operator const VALUE* (void) const		// 외부에서 락을 걸고 값을 직접 억세스 할때 사용함
		{
			if (count > 0)
				return &data[begin];
			return NULL;
		}
		size_t Count(void) const
		{
			CriticalSection guard(lock);
			return count;
		}
		size_t CountNoLock(void) const		// 외부에서 락을 걸었을때 사용함.
		{
			return count;
		}
		DKQueue& operator = (DKQueue&& queue)
		{
			if (this != &queue)
			{
				CriticalSection guard(lock);
				CriticalSection guard2(queue.lock);
				
				if (data && count)
				{
					for (size_t i = begin; i < begin+count; i++)
					{
						data[i].~VALUE();
					}
				}
				if (data)
					Allocator::Free(data);

				begin = queue.begin;
				count = queue.count;
				maxSize = queue.maxSize;
				data = queue.data;

				queue.begin = 0;
				queue.count = 0;
				queue.maxSize = 0;
				queue.data = NULL;
			}
			return *this;
		}
		DKQueue& operator = (const DKQueue& queue)
		{
			if (this != &queue)
			{
				Clear();
				PushBack(queue);
			}
			return *this;
		}
		DKQueue& operator = (std::initializer_list<VALUE> il)
		{
			Clear();
			PushBack(il);
			return *this;
		}
		// lambda enumerator (VALUE&) 또는 (VALUE&, bool*) 형식의 함수객체
		template <typename T> void EnumerateForward(T&& enumerator)
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum { ValidatePType1 = Func::template CanInvokeWithParameterTypes<VALUE&>::Result };
			enum { ValidatePType2 = Func::template CanInvokeWithParameterTypes<VALUE&, bool*>::Result };
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (VALUE&) or (VALUE&,bool*)");

			EnumerateForward(std::forward<T>(enumerator), typename Func::ParameterNumber());
		}
		template <typename T> void EnumerateBackward(T&& enumerator)
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum { ValidatePType1 = Func::template CanInvokeWithParameterTypes<VALUE&>::Result };
			enum { ValidatePType2 = Func::template CanInvokeWithParameterTypes<VALUE&, bool*>::Result };
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (VALUE&) or (VALUE&,bool*)");

			EnumerateBackward(std::forward<T>(enumerator), typename Func::ParameterNumber());
		}
		// lambda enumerator (const VALUE&) 또는 (const VALUE&, bool*) 형식의 함수객체
		template <typename T> void EnumerateForward(T&& enumerator) const
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum { ValidatePType1 = Func::template CanInvokeWithParameterTypes<const VALUE&>::Result };
			enum { ValidatePType2 = Func::template CanInvokeWithParameterTypes<const VALUE&, bool*>::Result };
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (const VALUE&) or (const VALUE&,bool*)");

			EnumerateForward(std::forward<T>(enumerator), typename Func::ParameterNumber());
		}
		template <typename T> void EnumerateBackward(T&& enumerator) const
		{
			using Func = typename DKFunctionType<T&&>::Signature;
			enum { ValidatePType1 = Func::template CanInvokeWithParameterTypes<const VALUE&>::Result };
			enum { ValidatePType2 = Func::template CanInvokeWithParameterTypes<const VALUE&, bool*>::Result };
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (const VALUE&) or (const VALUE&,bool*)");

			EnumerateBackward(std::forward<T>(enumerator), typename Func::ParameterNumber());
		}
	private:
		// lambda enumerator (VALUE&, bool*)
		template <typename T> void EnumerateForward(T&& enumerator, DKNumber<1>)
		{
			CriticalSection guard(lock);
			for (size_t i = 0; i < count; ++i)
				enumerator(data[begin+i]);
		}
		template <typename T> void EnumerateBackward(T&& enumerator, DKNumber<1>)
		{
			CriticalSection guard(lock);
			for (size_t i = 1; i <= count; ++i)
				enumerator(data[begin+count-i]);
		}
		// lambda enumerator (const VALUE&, bool*)
		template <typename T> void EnumerateForward(T&& enumerator, DKNumber<1>) const
		{
			CriticalSection guard(lock);
			for (size_t i = 0; i < count; ++i)
				enumerator(data[begin+i]);
		}
		template <typename T> void EnumerateBackward(T&& enumerator, DKNumber<1>) const
		{
			CriticalSection guard(lock);
			for (size_t i = 1; i <= count; ++i)
				enumerator(data[begin+count-i]);
		}
		// lambda enumerator (VALUE&, bool*)
		template <typename T> void EnumerateForward(T&& enumerator, DKNumber<2>)
		{
			CriticalSection guard(lock);
			bool stop = false;
			for (size_t i = 0; i < count && !stop; ++i)
				enumerator(data[begin+i], &stop);
		}
		template <typename T> void EnumerateBackward(T&& enumerator, DKNumber<2>)
		{
			CriticalSection guard(lock);
			bool stop = false;
			for (size_t i = 1; i <= count && !stop; ++i)
				enumerator(data[begin+count-i], &stop);
		}
		// lambda enumerator (const VALUE&, bool*)
		template <typename T> void EnumerateForward(T&& enumerator, DKNumber<2>) const
		{
			CriticalSection guard(lock);
			bool stop = false;
			for (size_t i = 0; i < count && !stop; ++i)
				enumerator(data[begin+i], &stop);
		}
		template <typename T> void EnumerateBackward(T&& enumerator, DKNumber<2>) const
		{
			CriticalSection guard(lock);
			bool stop = false;
			for (size_t i = 1; i <= count && !stop; ++i)
				enumerator(data[begin+count-i], &stop);
		}
		void ReserveFront(size_t n)
		{
			if (begin > n)
				return;

			size_t reqSize = maxSize+n;
			size_t newSize = maxSize > InitialSize ? maxSize : InitialSize;
			while (reqSize > newSize)
				newSize += newSize / 2;

			VALUE* dataNew = (VALUE*)Allocator::Alloc(sizeof(VALUE) * newSize);
			size_t offset = newSize - maxSize;
			if (data)
			{
				if (count)
					memcpy(&dataNew[begin+offset], &data[begin], sizeof(VALUE) * count);
				Allocator::Free(data);
			}
			maxSize += offset;
			begin += offset;
			data = dataNew;
		}
		void ReserveBack(size_t n)
		{
			if (maxSize >= begin + count + n)
				return;

			size_t reqSize = begin+count+n;
			size_t newSize = maxSize > InitialSize ? maxSize : InitialSize;
			while (reqSize > newSize)
				newSize += newSize / 2;

			VALUE* dataNew = (VALUE*)Allocator::Alloc(sizeof(VALUE) * newSize);
			if (data)
			{
				if (count)
					memcpy(&dataNew[begin], &data[begin], sizeof(VALUE) * count);
				Allocator::Free(data);
			}
			maxSize = newSize;
			data = dataNew;
		}
		void Balance(void)
		{
			size_t frontSpace = begin;
			size_t backSpace = maxSize - (begin+count);
			size_t begin2 = begin;

			if ((frontSpace > (backSpace + count) * 2) || (backSpace > (frontSpace + count) * 2))		// 앞뒤 공간차이가 나머지의 두배보다 크게 난다.
				begin2 = maxSize / 2;

			if (begin2 != begin)
			{
				if (count > 0)
					memmove(&data[begin2], &data[begin], sizeof(VALUE) * count);
				begin = begin2;
			}
		}

		size_t	begin;
		size_t	count;
		size_t	maxSize;
		VALUE* data;
	};
}
