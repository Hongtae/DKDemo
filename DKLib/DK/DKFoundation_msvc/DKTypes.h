//
//  File: DKTypes.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include <type_traits>
#include <utility>

////////////////////////////////////////////////////////////////////////////////
// 템플릿에서 사용되는 기본 타입들

namespace DKFoundation
{
	template <typename T> struct DKTypeTraitsCppExt
	{
		// compiler extensions
		// MSC: http://msdn.microsoft.com/en-us/library/vstudio/ms177194.aspx
		// GCC: http://gcc.gnu.org/onlinedocs/gcc/Type-Traits.html
		// Clang : http://clang.llvm.org/docs/LanguageExtensions.html

		enum { HasNothrowAssign = 					__has_nothrow_assign(T)};
		enum { HasNothrowCopy = 					__has_nothrow_copy(T)};
		enum { HasNothrowConstructor = 				__has_nothrow_constructor(T)};
		enum { HasTrivialAssign = 					__has_trivial_assign(T)};
		enum { HasTrivialCopy = 					__has_trivial_copy(T)};
		enum { HasTrivialConstructor = 				__has_trivial_constructor(T)};
		enum { HasTrivialDestructor = 				__has_trivial_destructor(T)};
		enum { HasVirtualDestructor = 				__has_virtual_destructor(T)};
		enum { IsAbstract = 						__is_abstract(T)};
		enum { IsClass = 							__is_class(T)};
		enum { IsEmpty = 							__is_empty(T)};
		enum { IsEnum = 							__is_enum(T)};
		enum { IsPod = 								__is_pod(T)};
		enum { IsPolymorphic = 						__is_polymorphic(T)};
		enum { IsUnion = 							__is_union(T)};
		template <typename U> struct IsBaseOf
		{
			enum { Result = __is_base_of(U, T) }; // __is_base_of(base, derived)
		};
	};

	// type number
	template <int i> struct DKNumber {enum {Value = i};	};
	// boolean type value (for SFINAE)
	using DKTrue = DKNumber<true>;
	using DKFalse = DKNumber<false>;

	namespace Private
	{
		template <typename T, bool isPolymorphic = DKTypeTraitsCppExt<T>::IsPolymorphic> struct BaseAddress;
		template <typename T> struct BaseAddress<T, true>
		{
			enum {isPolymorphic = 1};
			static void* Cast(T* p)
			{
				return dynamic_cast<void*>(p);
			}
		};
		template <typename T> struct BaseAddress<T, false>
		{
			enum {isPolymorphic = 0};
			static void* Cast(T* p)
			{
				return static_cast<void*>(p);
			}
		};

		// conditional type
		template <bool C, typename T, typename U> struct CondType
		{
			using Result = T;
		};
		template <typename T, typename U> struct CondType<false, T, U>
		{
			using Result = U;
		};
		// conditional enum
		template <bool C, int N1, int N2> struct CondEnum
		{
			enum {Result = N1};
		};
		template <int N1, int N2> struct CondEnum<false, N1, N2>
		{
			enum {Result = N2};
		};
		////////////////////////////////////////////////////////////////////////////////
		// Ns 들의 합계
		// _Sum<1,2,3>::Result = 6
		template <int... Ns> struct Sum
		{
			enum {Result = 0};
		};
		template <int N1, int... Ns> struct Sum<N1, Ns...>
		{
			enum {Result = N1 + Sum<Ns...>::Result};
		};
		// 타입T1 을 T2 로 변환 가능한지 여부 확인
		template <typename T1, typename T2> struct ConversionTest
		{
			template <typename U> struct _IsPointerOrReference		{enum {Result = false};};
			template <typename U> struct _IsPointerOrReference<U*>	{enum {Result = true};};
			template <typename U> struct _IsPointerOrReference<U&>	{enum {Result = true};};
			template <typename U> struct _IsPointerOrReference<U&&>	{enum {Result = true};};

			// 타입이 포인터이거나 레퍼런스이면 그냥 쓰고 그외엔 레퍼런스로 바꾼다.
			using Type1Ref = typename CondType<_IsPointerOrReference<T1>::Result, T1, T1&>::Result;
			using Type2Ref = typename CondType<_IsPointerOrReference<T2>::Result, T2, const T2&>::Result;

			static DKTrue _Test(Type2Ref);
			static DKFalse _Test(...);
			static Type1Ref _TypeSub();

			enum {Result = decltype(_Test(_TypeSub()))::Value};
		};
		template <typename T> struct ConversionTest<T, T>		{enum {Result = true};};
		template <typename T> struct ConversionTest<void, T>	{enum {Result = false};};
		template <typename T> struct ConversionTest<T, void>	{enum {Result = false};};
		template <> struct ConversionTest<void, void>			{enum {Result = true};};
	}

	// C 가 true 이면 T, 아니면 U 가 된다.
	template <bool C, typename T, typename U> using DKCondType = typename Private::CondType<C, T, U>::Result;

	// C 가 true 면 상수 N1, 아니면 N2 를 리턴한다. (constexpr)
	template <bool C, int N1, int N2> struct DKCondEnum
	{
		enum { Result = Private::CondEnum<C, N1, N2>::Result };
	};

	// Ns 의 합계를 리턴한다 (constexpr)
	template <int... Ns> struct DKSum
	{
		enum { Result = Private::Sum<Ns...>::Result };
	};

	// Source 가 Target 으로 형변환이 가능한지 확인
	template <typename Source, typename Target> struct DKTypeConversionTest
	{
		enum { Result = Private::ConversionTest<Source, Target>::Result };
	};
	
	// 객체의 시작 주소를 리턴한다. (다중상속의 경우 시작 주소가 달라질 수 있다)
	template <typename T> void* DKTypeBaseAddressCast(T* p)
	{
		return Private::BaseAddress<T>::Cast(p);
	}
}
