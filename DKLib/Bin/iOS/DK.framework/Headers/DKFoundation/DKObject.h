//
//  File: DKObject.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKTypes.h"
#include "DKTypeTraits.h"
#include "DKAllocator.h"
#include "DKObjectRefCounter.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKObject
//
// 객체타입을 스칼라타입으로 변형하여 사용할때 필요하다.
// (객체의 포인터를 포함하며 스칼라 타입으로 사용할 수 있다.)
//
// DKAllocator 클래스를 통하여 자동으로 참조 관리를 한다.
//
// DKAllocator 를 통하여 생성된 객체는 자동으로 참조 관리가 된다.
//
// 객체 생성 방법
// 1. DKObject::New()
//     DKObject<OBJECT> p1 = DKObject<OBJECT>::New();
//     DKObject<OBJECT> p2 = DKObject<OBJECT>::New( arg1, arg2 ...);
//
// 2. DKOBJECT_NEW 매크로
//     DKObject<OBJECT> p1 = DKOBJECT_NEW OBJECT();
//     DKObject<OBJECT> p2 = DKOBJECT_NEW OBJECT( arg1, arg2, ...);
//
// 2. DKObject::Alloc()
//  - 특수한 allocator 사용시 사용한다.
//     DKObject<OBJECT> p1 = DKObject<OBJECT>::Alloc( myAllocator );
//     DKObject<OBJECT> p1 = DKObject<OBJECT>::Alloc( myAllocator, ...);
//
// DKObject::New
// 1. 파라메터가 없을때 (기본 생성자)
//		DKObject<OBJECT> p = DKObject<OBJECT>::Alloc();
// 2. 복사생성자 이용
//		DKObject<OBJECT> p = DKObject<OBJECT>::Alloc(obj);
// 3. 파라메터가 여러개인 생성자를 가진 객체 생성 (DKOBJECT_NEW 매크로 사용)
//		DKObject<OBJECT> p = DKOBJECT_NEW OBJECT(p1,p2,..);
//
// weak-ref 사용법
// DKObject<OBJECT> p = DKObject<OBJECT>::New();
// DKObject<OBJECT>::Ref ref = p;
//  p = NULL;	// 객체 소거됨.
// DKObject<OBJECT> p2 = ref;	// p2 = NULL
//
// 주의:
//  1. DKObject<void> 는 사용할 수 없다!
//  2. 다중 상속의 경우 polymorphic 타입(virtual 함수가존재)이 아닐경우 형변환시 객체가 상실될 수 있다.
//
////////////////////////////////////////////////////////////////////////////////

#define DKOBJECT_NEW			new(DKFoundation::DKAllocator::DefaultAllocator())

namespace DKFoundation
{
	template <typename T> class DKObject
	{
	public:
		using TypeTraits = DKTypeTraits<T>;
		static_assert( TypeTraits::IsReference == 0, "Reference type cannot be used!");

		constexpr static bool IsPolymorphic() {return TypeTraits::IsPolymorphic();}
		
		using RefCounter = DKObjectRefCounter;
		class Ref
		{
		public:
			Ref(void) : ptr(NULL), refId(0) {}
			Ref(const Ref& r) : ptr(r.ptr), refId(r.refId) {}
			Ref& operator = (const Ref& ref)
			{
				ptr = ref.ptr;
				refId = ref.refId;
				return *this;
			}
		private:
			T* ptr;
			RefCounter::RefIdValue refId;
			friend class DKObject;
		};
		DKObject(T* p = NULL) : _target(_RetainObject(p))
		{
		}
		DKObject(DKObject&& obj) : _target(NULL)
		{
			_target = obj._target;
			obj._target = NULL;
		}
		DKObject(const DKObject& obj) : _target(_RetainObject(obj._target))
		{
		}
		DKObject(const Ref& ref) : _target(_RetainObject(ref))
		{
		}
		~DKObject(void)
		{
			_ReleaseObject(_target);
		}
		// 포인터 연산자
		T* operator ->(void)						{return _target;}
		const T* operator ->(void) const			{return _target;}
		T& operator * (void)						{return *_target;}
		const T& operator * (void) const			{return *_target;}
		// 형변환
		operator T* (void)							{return _target;}
		operator const T* (void) const				{return _target;}

		T* Ptr(void)								{return _target;}
		const T* Ptr(void) const					{return _target;}

		template <typename R> constexpr static bool IsConvertible(void)
		{
			return DKTypeConversionTest<T, R>();
		}
		template <typename R> R* SafeCast(void)
		{
			if (IsConvertible<R>())							// up casting
				return static_cast<R*>(_target);
			return dynamic_cast<R*>(_target);		// else down casting
		}
		template <typename R> const R* SafeCast(void) const
		{
			if (IsConvertible<R>())									// up casting
				return static_cast<const R*>(_target);
			return dynamic_cast<const R*>(_target);		// else down casting
		}
		template <typename R> R* StaticCast(void)
		{
			return static_cast<R*>(_target);
		}
		template <typename R> const R* StaticCast(void) const
		{
			return static_cast<const R*>(_target);
		}
		template <typename R> R* ReinterpretCast(void)
		{
			return reinterpret_cast<R*>(_target);
		}
		template <typename R> const R* ReinterpretCast(void) const
		{
			return reinterpret_cast<const R*>(_target);
		}
		DKObject& operator = (DKObject&& obj)
		{
			if (_target != obj._target)
			{
				T* tmp = _target;
				_target = obj._target;
				obj._target = NULL;
				_ReleaseObject(tmp);
			}
			return *this;
		}
		DKObject& operator = (const DKObject& obj)
		{
			return operator = (obj._target);
		}
		DKObject& operator = (T* p)
		{
			if (_target != p)
			{
				T* old = _target;
				_target = _RetainObject(p);
				_ReleaseObject(old);
			}
			return *this;
		}
		DKObject& operator = (const Ref& ref)
		{
			if (_target != ref.ptr)
			{
				T* old = _target;
				_target = _RetainObject(ref);
				_ReleaseObject(old);
			}
			return *this;
		}
		operator Ref (void) const		// Ref 로 형변환
		{
			Ref ref;
			RefCounter::RefIdValue refId;
			if (_target && RefCounter::RefId(BaseAddress(_target), &refId))
			{
				ref.ptr = _target;
				ref.refId = refId;
			}
			return ref;
		}

		template <typename... Args> static DKObject Alloc(DKAllocator& alloc, Args&&... args)
		{
			return new(alloc) T(std::forward<Args>(args)...);
		}
		template <typename... Args> static DKObject New(Args&&... args)
		{
			return new(DKAllocator::DefaultAllocator(DKMemoryLocationHeap)) T(std::forward<Args>(args)...);
		}
		DKAllocator* Allocator(void) const
		{
			if (_target)
				return RefCounter::Allocator(BaseAddress(_target));
			return NULL;
		}
		bool IsManaged(void) const
		{
			if (_target && RefCounter::RefId(BaseAddress(_target), NULL))
				return true;
			return false;
		}
		bool IsShared(void) const
		{
			RefCounter::RefCountValue ref = 0;
			if (_target && RefCounter::RefCount(BaseAddress(_target), &ref))
				return ref > 1;
			return false;
		}
		RefCounter::RefCountValue SharingCount(void) const
		{
			RefCounter::RefCountValue ref = 0;
			if (_target && RefCounter::RefCount(BaseAddress(_target), &ref))
				return ref;
			return 0;
		}
		// polymorphic determine
		void* BaseAddress(void) const
		{
			return BaseAddress(_target);
		}
	private:
		static T* _RetainObject(const Ref& ref)
		{
			if (ref.ptr && RefCounter::IncrementRefCount(BaseAddress(ref.ptr), ref.refId))
				return ref.ptr;
			return NULL;
		}
		static T* _RetainObject(T* p)
		{
			if (p)
				RefCounter::IncrementRefCount(BaseAddress(p));
			return p;
		}
		static void _ReleaseObject(T* p)
		{
			if (p)
			{
				// 참조값 감소하고 0 이면 제거함.
				DKAllocator* allocator = NULL;
				if (RefCounter::DecrementRefCountAndUnsetIfZero(BaseAddress(p), &allocator))
				{
					if (allocator)
					{
						// destructor 호출 전에 미리 base address 구해놔야 한다.
						void* ptr = BaseAddress(p);
						p->~T();
						allocator->Dealloc(ptr);
					}
				}
			}
		}
		static void* BaseAddress(T* p)
		{
			return DKTypeBaseAddressCast<T>(p);
		}
		T* _target;
	};
	template <typename T> class DKObject<T*>;
	template <typename T> class DKObject<T&>;
	template <typename T> class DKObject<T&&>;

	class DKUnknown
	{
	public:
		virtual ~DKUnknown(void) {}
	};
}
