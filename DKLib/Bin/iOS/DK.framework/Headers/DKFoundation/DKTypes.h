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

		constexpr static bool HasNothrowAssign()				{return __has_nothrow_assign(T);}
		constexpr static bool HasNothrowCopy()					{return __has_nothrow_copy(T);}
		constexpr static bool HasNothrowConstructor()			{return __has_nothrow_constructor(T);}
		constexpr static bool HasTrivialAssign()				{return __has_trivial_assign(T);}
		constexpr static bool HasTrivialCopy()					{return __has_trivial_copy(T);}
		constexpr static bool HasTrivialConstructor()			{return __has_trivial_constructor(T);}
		constexpr static bool HasTrivialDestructor()			{return __has_trivial_destructor(T);}
		constexpr static bool HasVirtualDestructor()			{return __has_virtual_destructor(T);}
		constexpr static bool IsAbstract()						{return __is_abstract(T);}
		constexpr static bool IsClass()							{return __is_class(T);}
		constexpr static bool IsEmpty()							{return __is_empty(T);}
		constexpr static bool IsEnum()							{return __is_enum(T);}
		constexpr static bool IsPod()							{return __is_pod(T);}
		constexpr static bool IsPolymorphic()					{return __is_polymorphic(T);}
		constexpr static bool IsUnion()							{return __is_union(T);}
		template <typename U> constexpr static bool IsBaseOf()	{return __is_base_of(U, T);} // __is_base_of(base, derived)
	};

	// type number
	template <int i> struct DKNumber {enum {Value = i};	};
	// boolean type value (for SFINAE)
	using DKTrue = DKNumber<true>;
	using DKFalse = DKNumber<false>;

	namespace Private
	{
		template <typename T, bool isPolymorphic = DKTypeTraitsCppExt<T>::IsPolymorphic()> struct BaseAddress;
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
	template <bool C, int N1, int N2> constexpr int DKCondEnum(void)
	{
		return Private::CondEnum<C, N1, N2>::Result;
	}

	// Ns 의 합계를 리턴한다 (constexpr)
	template <int... Ns> constexpr int DKSum(void)
	{
		return Private::Sum<Ns...>::Result;
	}

	// Source 가 Target 으로 형변환이 가능한지 확인
	template <typename Source, typename Target> constexpr bool DKTypeConversionTest(void)
	{
		return Private::ConversionTest<Source, Target>::Result;
	}
	
	// 객체의 시작 주소를 리턴한다. (다중상속의 경우 시작 주소가 달라질 수 있다)
	template <typename T> void* DKTypeBaseAddressCast(T* p)
	{
		return Private::BaseAddress<T>::Cast(p);
	}
}
