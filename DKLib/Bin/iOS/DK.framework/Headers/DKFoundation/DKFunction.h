//
//  File: DKFunction.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKTuple.h"
#include "DKTypeTraits.h"
#include "DKInvocation.h"
#include "DKObject.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKFunctionSignature
//
// 함수객체 형식, 템플릿 파라메터는 함수여야 한다. (예: DKFunctionSignature<void (int)>)
//
// 아래의 함수를 이용하여 DKFunctionSignature 객체를 생성할 수 있다.
// DKFunction(일반 함수 포인터)
// DKFunction(함수객체)
// DKFunction(객체 레퍼런스 또는 포인터, 객체의 멤버함수 포인터)
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	// DKFunctionSignature 인터페이스
	template <typename Function> struct DKFunctionSignature;
	template <typename R, typename... Ps> struct DKFunctionSignature<R (Ps...)>
	{
		using ParameterNumber = DKNumber<sizeof...(Ps)>;
		using ParameterTuple = DKTuple<Ps...>;
		using ReturnType = R;

		// direct call
		virtual ReturnType Invoke(Ps...) const = 0;
		virtual ReturnType InvokeWithTuple(ParameterTuple&) const = 0;

		// indirect-invocation call
		virtual DKObject<DKInvocation<R>> Invocation(Ps...) const = 0;
		virtual DKObject<DKInvocation<R>> InvocationWithTuple(ParameterTuple&) const = 0;
		virtual DKObject<DKInvocation<R>> InvocationWithTuple(ParameterTuple&&) const = 0;

		virtual ~DKFunctionSignature(void) {}

		template <typename... Ts> constexpr static bool CanInvokeWithParameterTypes(void)
		{
			return DKTypeList<Ts...>::template IsConvertible<typename ParameterTuple::TypeList>::Value;
		};
	};

	namespace Private
	{
		////////////////////////////////////////////////////////////////////////////////
		// 튜플을 함수의 파라메터로 넣어주는 클래스
		template <int N, int Count, int... Ns> struct _TupleInvoker
		{
			using Result = typename _TupleInvoker<N+1, Count-1, Ns..., N>::Result;
		};
		template <int N, int... Ns> struct _TupleInvoker<N, 0, Ns...>
		{
			struct _Result
			{
				// 일반 함수 포인터, 또는 함수객체
				template <typename Ret, typename Func, typename Tuple>
				static auto Invoke(Func&& fn, Tuple&& tuple)->Ret
				{
					return fn( tuple.template Value<Ns>()... );
				}
				// 클래스 멤버 함수 (클래스 인스턴스는 포인터로 사용함)
				template <typename Ret, class Obj, typename Func, typename Tuple>
				static auto Invoke(Obj&& obj, Func fn, Tuple&& tuple)->Ret
				{
					return (obj.*fn)( tuple.template Value<Ns>()... );
				}
			};
			using Result = _Result;
		};
		template <size_t Length> using TupleInvoker = typename _TupleInvoker<0, Length>::Result;

		////////////////////////////////////////////////////////////////////////////////
		// 일반 함수, 함수객체의 Invocation 을 생성하는 객체 (DKFunctionSignature 의 implementation)
		template <typename Function, typename R, typename... Ps> class FunctionObjectInvoker
		: public DKFunctionSignature<R (Ps...)>
		{
		public:
			using Signature = DKFunctionSignature<R (Ps...)>;
			using ReturnType = typename Signature::ReturnType;
			using ParameterTuple = typename Signature::ParameterTuple;

			struct _Invocation : public DKInvocation<R>
			{
				_Invocation(Function& f) : function(f) {}
				_Invocation(Function& f, ParameterTuple& t) : function(f), tuple(t) {}
				_Invocation(Function& f, ParameterTuple&& t) : function(f), tuple(static_cast<ParameterTuple&&>(t)) {}
				Function function;
				ParameterTuple tuple;
				R Invoke(void) const override
				{
					Function& fn = const_cast<_Invocation&>(*this).function;
					ParameterTuple& tup = const_cast<_Invocation&>(*this).tuple;
					return TupleInvoker<ParameterTuple::Length>::template Invoke<R>(fn, tup);
				}
			};

			ReturnType Invoke(Ps... vs) const override
			{
				Function& fn = const_cast<FunctionObjectInvoker&>(*this).function;
				return fn(std::forward<Ps>(vs)...);
			}
			ReturnType InvokeWithTuple(ParameterTuple& tuple) const override
			{
				Function& fn = const_cast<FunctionObjectInvoker&>(*this).function;
				return TupleInvoker<ParameterTuple::Length>::template Invoke<ReturnType>(fn, tuple);
			}
			DKObject<DKInvocation<R>> Invocation(Ps... vs) const override
			{
				Function& fn = const_cast<FunctionObjectInvoker&>(*this).function;
				DKObject<_Invocation> inv = DKOBJECT_NEW _Invocation(fn, ParameterTuple(DKTupleValueSet(), std::forward<Ps>(vs)...));
				return inv.template SafeCast<DKInvocation<R>>();
			}
			DKObject<DKInvocation<R>> InvocationWithTuple(ParameterTuple& tuple) const override
			{
				Function& fn = const_cast<FunctionObjectInvoker&>(*this).function;
				DKObject<_Invocation> inv = DKOBJECT_NEW _Invocation(fn, tuple);
				return inv.template SafeCast<DKInvocation<R>>();
			}
			DKObject<DKInvocation<R>> InvocationWithTuple(ParameterTuple&& tuple) const override
			{
				Function& fn = const_cast<FunctionObjectInvoker&>(*this).function;
				DKObject<_Invocation> inv = DKOBJECT_NEW _Invocation(fn, static_cast<ParameterTuple&&>(tuple));
				return inv.template SafeCast<DKInvocation<R>>();
			}

			FunctionObjectInvoker(Function& fn) : function(fn) {}
			FunctionObjectInvoker(Function&& fn) : function(static_cast<Function&&>(fn)) {}
			Function function;
		};
		////////////////////////////////////////////////////////////////////////////////
		// 클래스 멤버함수 Invocation 생성하는 객체 (DKFunctionSignature 의 implementation)
		template <class Object, typename Function, typename R, typename... Ps> class FunctionMemberObjectInvoker
		: public DKFunctionSignature<R (Ps...)>
		{
		public:
			using Signature = DKFunctionSignature<R (Ps...)>;
			using ReturnType = typename Signature::ReturnType;
			using ParameterTuple = typename Signature::ParameterTuple;

			struct _Invocation : public DKInvocation<R>
			{
				_Invocation(Object& obj, Function f) : object(obj), function(f) {}
				_Invocation(Object& obj, Function f, ParameterTuple& t) : object(obj), function(f), tuple(t) {}
				_Invocation(Object& obj, Function f, ParameterTuple&& t) : object(obj), function(f), tuple(static_cast<ParameterTuple&&>(t)) {}
				Object object;
				Function function;
				ParameterTuple tuple;
				R Invoke(void) const override
				{
					Object& obj = const_cast<_Invocation&>(*this).object;
					ParameterTuple& tup = const_cast<_Invocation&>(*this).tuple;
					return TupleInvoker<ParameterTuple::Length>::template Invoke<R>(*obj, function, tup);
				}
			};

			ReturnType Invoke(Ps... vs) const override
			{
				Object& obj = const_cast<FunctionMemberObjectInvoker&>(*this).object;
				return ((*obj).*function)(std::forward<Ps>(vs)...);
			}
			ReturnType InvokeWithTuple(ParameterTuple& tuple) const override
			{
				Object& obj = const_cast<FunctionMemberObjectInvoker&>(*this).object;
				return TupleInvoker<ParameterTuple::Length>::template Invoke<ReturnType>(*obj, function, tuple);
			}
			DKObject<DKInvocation<R>> Invocation(Ps... vs) const override
			{
				Object& obj = const_cast<FunctionMemberObjectInvoker&>(*this).object;
				DKObject<_Invocation> inv = DKOBJECT_NEW _Invocation(obj, function, ParameterTuple(DKTupleValueSet(), std::forward<Ps>(vs)...));
				return inv.template SafeCast<DKInvocation<R>>();
			}
			DKObject<DKInvocation<R>> InvocationWithTuple(ParameterTuple& tuple) const override
			{
				Object& obj = const_cast<FunctionMemberObjectInvoker&>(*this).object;
				DKObject<_Invocation> inv = DKOBJECT_NEW _Invocation(obj, function, tuple);
				return inv.template SafeCast<DKInvocation<R>>();
			}
			DKObject<DKInvocation<R>> InvocationWithTuple(ParameterTuple&& tuple) const override
			{
				Object& obj = const_cast<FunctionMemberObjectInvoker&>(*this).object;
				DKObject<_Invocation> inv = DKOBJECT_NEW _Invocation(obj, function, static_cast<ParameterTuple&&>(tuple));
				return inv.template SafeCast<DKInvocation<R>>();
			}

			using ObjectTraits = DKTypeTraits<Object>;
			// Object 타입은 T*, const T*, DKObject<T>, Wrapper<T>, Wrapper<const T> 여야한다.
			// 레퍼런스타입은 올수 없다.
			static_assert(ObjectTraits::IsReference == 0, "Object must not be a reference type!");

			using UnqualifedObjectType = typename ObjectTraits::UnqualifiedReferredType;
			FunctionMemberObjectInvoker(UnqualifedObjectType& obj, Function fn) : object(obj), function(fn) {}
			FunctionMemberObjectInvoker(UnqualifedObjectType&& obj, Function fn) : object(obj), function(fn) {}
			FunctionMemberObjectInvoker(const UnqualifedObjectType& obj, Function fn) : object(obj), function(fn) {}

			Object object;
			Function function;
		};

		////////////////////////////////////////////////////////////////////////////////
		// 일반 함수 또는 함수 객체의 DKFunctionSignature (implements) 객체를 판별하는 클래스
		template <typename T> struct FunctionTypeSelector			// 함수객체
		{
			template <typename U> static DKTrue HasFunctionOperator(decltype(&U::operator()));
			template <typename U> static DKFalse HasFunctionOperator(...);
			enum {IsFunctionObject = decltype(HasFunctionOperator<T>(0))::Value};

			static_assert( IsFunctionObject, "Type is not function object!");

			using FunctionTraits = DKMemberFunctionPointerTraits<decltype(&T::operator())>;
			using ReturnType = typename FunctionTraits::ReturnType;
			using ParameterTypeList = typename FunctionTraits::ParameterTypeList;
			template <typename... Ts> using _Invoker = FunctionObjectInvoker<T, ReturnType, Ts...>;
			using Invoker = typename ParameterTypeList::template TypesInto<_Invoker>;
			using Signature = typename Invoker::Signature;
		};
		template <typename T> struct FunctionTypeSelector<T*>		// 함수포인터
		{
			static_assert( DKTypeTraits<T*>::IsFunctionPointer, "Type is not function!");

			using FunctionTraits = DKFunctionPointerTraits<T*>;
			using ParameterTypeList = typename FunctionTraits::ParameterTypeList;
			template <typename... Ts> using _Invoker = FunctionObjectInvoker<T*, typename FunctionTraits::ReturnType, Ts...>;
			using Invoker = typename ParameterTypeList::template TypesInto<_Invoker>;
			using Signature = typename Invoker::Signature;
		};
		template <typename T, typename Func> struct MemberFunctionTypeSelector		// 멤버함수
		{
			static_assert( DKTypeTraits<Func>::IsMemberFunctionPointer, "Type is not member function!");

			using FunctionTraits = DKMemberFunctionPointerTraits<Func>;
			using ParameterTypeList = typename FunctionTraits::ParameterTypeList;
			template <typename... Ts> using _Invoker = FunctionMemberObjectInvoker<T, Func, typename FunctionTraits::ReturnType, Ts...>;
			using Invoker = typename ParameterTypeList::template TypesInto<_Invoker>;
			using Signature = typename Invoker::Signature;
		};

		////////////////////////////////////////////////////////////////////////////////
		// 일반 함수인지, 함수객체인지 판별함 (클래스 멤버함수는 판별하지 않음)
		template <typename T> struct IdentifyFunction
		{
			template <typename U> static DKTrue HasFunctionOperator(decltype(&U::operator()));
			template <typename U> static DKFalse HasFunctionOperator(...);
			template <typename U, bool hasOp> struct _FunctionTraits;
			template <typename U> struct _FunctionTraits<U, true>
			{
				using Traits = typename DKTypeTraits<decltype(&U::operator())>::MemberFunctionTraits;
				enum {IsObjectConst = DKTypeTraits<U>::IsConst};	// 객체가 const 인지
				enum {IsFunctionConst = Traits::IsConst};			// 함수가 const 인지
				// 객체가 const 면 함수도 const 여야 호출 가능하다.
				enum {IsCallable = DKCondEnum<IsObjectConst, IsFunctionConst, true>()};
			};
			template <typename U> struct _FunctionTraits<U, false>
			{
				using Traits = typename DKTypeTraits<U>::FunctionTraits;
				enum {IsCallable = Traits::IsFunctionPointer};
			};

			using Ref = typename DKTypeTraits<T>::ReferredType;
			using TypeTraits = DKTypeTraits<Ref>;
			enum {IsFunction = TypeTraits::IsFunction};
			enum {IsFunctionPointer = TypeTraits::IsFunctionPointer};
			enum {IsFunctionObject = decltype(HasFunctionOperator<Ref>(0))::Value};

			// 함수이면 함수포인터로 변경함
			using Callable = DKCondType<IsFunction, Ref*, Ref>;
			using FunctionTraits = typename _FunctionTraits<Callable, IsFunctionObject>::Traits;
			enum {IsCallable = _FunctionTraits<Callable, IsFunctionObject>::IsCallable};
		};

		////////////////////////////////////////////////////////////////////////////////
		// 멤버 함수 호출 가능여부 판별함.
		template <typename T, typename Func> struct IdentifyMemberFunction
		{
			template <typename U> struct _Wrapper
			{
				static_assert( DKTypeTraits<U>::IsPointer == 0, "Type must not be a pointer");
				static_assert( DKTypeTraits<U>::IsReference == 0, "Type must not be a reference");

				_Wrapper(U& o) : object(o)									{}
				_Wrapper(U&& o) : object(std::forward<U>(o))				{}
				_Wrapper(const _Wrapper& w) : object(w.object)				{}
				_Wrapper(_Wrapper&& w) : object(std::forward<U>(w.object))	{}

				U& operator * (void)	{return object;}

				_Wrapper& operator = (_Wrapper&&) = delete;
				_Wrapper& operator = (const _Wrapper&) = delete;

				U object;
			};

			template <typename U> struct _ObjectTraits
			{
				enum {IsPointer = DKTypeTraits<U>::IsPointer};
				using ObjectType = DKCondType<IsPointer, typename DKTypeTraits<U>::PointeeType, typename DKTypeTraits<U>::ReferredType>;
				enum {IsConst = DKTypeTraits<ObjectType>::IsConst};
				using Wrapper = DKCondType<IsPointer, ObjectType*, _Wrapper<ObjectType>>;
			};
			// special-case DKObject : DKObject 는 Wrapper 로 간주함.
			template <typename U> struct _ObjectTraits<DKObject<U>>
			{
				using ObjectType = U;
				enum {IsPointer = true};
				enum {IsConst = false};
				using Wrapper = DKObject<U>;
			};
			template <typename U> struct _ObjectTraits<DKObject<U>&>
			{
				using ObjectType = U;
				enum {IsPointer = true};
				enum {IsConst = false};
				using Wrapper = DKObject<U>;
			};
			template <typename U> struct _ObjectTraits<const DKObject<U>&>
			{
				using ObjectType = U;
				enum {IsPointer = true};
				enum {IsConst = true};
				using Wrapper = const DKObject<U>;
			};

			template <bool IsConst, typename U, typename F> struct _IsCallable
			{
				using FunctionTraits = DKMemberFunctionPointerTraits<F>;
				static_assert(FunctionTraits::IsFunctionPointer, "Type Func is not member function!");

				using ClassType = typename FunctionTraits::ClassType;
				using ObjectType = typename DKTypeTraits<U>::UnqualifiedReferredType;

				enum {IsObjectConst = IsConst};
				enum {IsFunctionConst = FunctionTraits::IsConst};

				// U 가 Func 를 호출 가능한지 확인.
				enum {ConversionTest = DKTypeConversionTest<ObjectType*, ClassType*>()};
				enum {ConstTest = DKCondEnum<IsObjectConst, IsFunctionConst, true>()};

				enum {Result = ConversionTest && ConstTest};
			};

			using FunctionTraits = typename DKTypeTraits<Func>::MemberFunctionTraits;
			using ObjectTraits = _ObjectTraits<T>;
			using PointerWrapper = typename ObjectTraits::Wrapper;

			enum {IsPointer = ObjectTraits::IsPointer};
			enum {IsConst = ObjectTraits::IsConst};
			enum {IsCallable = _IsCallable<ObjectTraits::IsConst, typename ObjectTraits::ObjectType, Func>::Result};
		};
	}

	template <typename T> using DKFunctionTest = Private::IdentifyFunction<T>;
	template <typename T> using DKFunctionType = Private::FunctionTypeSelector<typename DKFunctionTest<T>::Callable>;

	// functor or function pointer
	template <typename Func> auto DKFunction(Func&& fn)-> DKObject<typename DKFunctionType<Func>::Signature>
	{
		using FunctionTest = DKFunctionTest<Func>;
		using FunctionTraits = typename FunctionTest::FunctionTraits;

		static_assert( FunctionTraits::IsFunctionPointer, "Type is not a function!");
		static_assert( FunctionTest::IsCallable, "Function is not callable");

		using FunctionType = DKFunctionType<Func>;
		using Invoker = typename FunctionType::Invoker;
		using Signature = typename FunctionType::Signature;

		return DKOBJECT_NEW Invoker(std::forward<Func>(fn));
	}

	template <typename T, typename Func> using DKFunctionMemberTest = Private::IdentifyMemberFunction<T, Func>;
	template <typename T, typename Func> using DKFunctionMemberType = Private::MemberFunctionTypeSelector<typename DKFunctionMemberTest<T, Func>::PointerWrapper, Func>;

	// class-member function (obj: object ptr or DKObject)
	template <typename T, typename Func> auto DKFunction(T&& obj, Func&& fn)-> DKObject<typename DKFunctionMemberType<T, Func>::Signature>
	{
		using FunctionTest = DKFunctionMemberTest<T, Func>;
		using FunctionTraits = typename FunctionTest::FunctionTraits;

		static_assert( DKTypeTraits<Func>::IsMemberPointer, "Type(Func) is not member pointer");
		static_assert( DKTypeTraits<Func>::IsMemberFunctionPointer, "Type(Func) is not member function!");
		static_assert( FunctionTest::IsCallable, "Member function is not callable!");

		using FunctionType = DKFunctionMemberType<T, Func>;
		using Invoker = typename FunctionType::Invoker;
		using Signature = typename FunctionType::Signature;

		return DKOBJECT_NEW Invoker(std::forward<T>(obj), fn);
	}
}
