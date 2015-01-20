//
//  File: DKTypeList.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKTypes.h"

namespace DKFoundation
{
	template <typename... Types> class DKTypeList
	{
	public:
		// 리스트의 Index에 위치한 타입을 Result 로 재정의함 (범위를 벗어나면 에러)
		template <unsigned Index> struct _TypeAt
		{
			// Index 를 하나씩 줄여가며, 0 이됐을때의 타입을 정의함.
			template <unsigned N, typename T, typename... Ts> struct _EnumTypes
			{
				using Result = typename _EnumTypes<N-1, Ts...>::Result;
			};
			template <typename T, typename... Ts> struct _EnumTypes<0, T, Ts...>
			{
				using Result = T;
			};

			static_assert(Index < sizeof...(Types), "Index must be less than count");
			using Result = typename _EnumTypes<Index, Types...>::Result;
		};
		// 리스트에 해당 타입T 의 위치를 구함. (존재하지 않으면 에러)
		template <typename T> struct _IndexOf
		{
			// 타입 T 가 나올때까지 Result 를 증가시킨다.
			template <typename, typename...> struct _StepMatchingType;
			template <typename T1, typename T2, typename... Ts> struct _StepMatchingType<T1, T2, Ts...>
			{
				enum {Result = 1 + _StepMatchingType<T1, Ts...>::Result};
			};
			template <typename T1, typename... Ts> struct _StepMatchingType<T1, T1, Ts...>
			{
				enum {Result = 0};
			};
			template <typename T1> struct _StepMatchingType<T1>
			{
				enum {Result = 0};
			};

			enum {Result = _StepMatchingType<T, Types...>::Result};
			static_assert(Result < sizeof...(Types), "Type is not in list");
		};
		// 타입T 가 리스트에 몇개 있는지 구함
		template <typename T> struct _CountType
		{
			// 모든 타입을 순회하며, 타입 T 가 나오면 Result 를 1씩 증가한다.
			template <typename, typename...> struct _Count;
			template <typename T1, typename T2, typename... Ts> struct _Count<T1, T2, Ts...>
			{
				enum {Result = _Count<T1, Ts...>::Result};
			};
			template <typename T1, typename... Ts> struct _Count<T1, T1, Ts...>
			{
				enum {Result = 1 + _Count<T1, Ts...>::Result};
			};
			template <typename T1> struct _Count<T1>
			{
				enum {Result = 0};
			};

			enum {Result = _Count<T, Types...>::Result};
		};

		// 리스트 뒤에 타입들 추가
		template <typename... Ts> struct _Append
		{
			using Result = DKTypeList<Types..., Ts...>;
		};
		// 리스트에 새 리스트 추가
		template <typename... Ts> struct _Append<DKTypeList<Ts...>>
		{
			using Result = DKTypeList<Types..., Ts...>;
		};
		// 리스트가 같은지 확인.
		template <typename... Ts> struct _IsSame
		{
			enum {Result = _IsSame<DKTypeList<Ts...>>::Result};
		};
		template <typename... Ts> struct _IsSame<DKTypeList<Ts...>>
		{
			template <typename T1, typename T2> struct IsSameType	{enum {Result = false};};
			template <typename T> struct IsSameType<T, T>			{enum {Result = true};};

			// 서로 다른 타입이 나올때까지 센다. (S 범위 내에서)
			template <unsigned S, typename L1, typename L2> struct _CountSameTypes
			{
				enum {Result =
						DKCondEnum<
							IsSameType< typename L1::template TypeAt<S-1>, typename L2::template TypeAt<S-1>>::Result,
							_CountSameTypes<S-1, L1, L2>::Result + 1,
							0
						>::Result};
			};
			template <typename L1, typename L2> struct _CountSameTypes<0, L1, L2>
			{
				enum {Result = 0};
			};

			enum {LengthA = sizeof...(Types), LengthB = sizeof...(Ts)};
			enum {MinRange = DKCondEnum< (LengthA < LengthB), LengthA, LengthB>::Result};

			using _List1 = DKTypeList<Types...>;
			using _List2 = DKTypeList<Ts...>;

			enum {NumSameTypes = _CountSameTypes<MinRange, _List1, _List2>::Result};
			enum {Result = NumSameTypes == LengthA && NumSameTypes == LengthB};
		};
		// 타입이 변형 가능한지 확인
		template <typename... Ts> struct _IsConvertible
		{
			enum { Result = _IsConvertible<DKTypeList<Ts...>>::Result };
		};
		template <typename... Ts> struct _IsConvertible<DKTypeList<Ts...>>
		{
			// 타입이 변형 불가능할때까지 센다. (S 범위 내에서)
			template <unsigned S, typename L1, typename L2> struct _CountCompatibles
			{
				enum {Index = S-1};
				using T1 = typename L1::template TypeAt<Index>;
				using T2 = typename L2::template TypeAt<Index>;
				enum { Result =
						DKCondEnum<
							DKTypeConversionTest<T1, T2>::Result,
							_CountCompatibles<Index, L1, L2>::Result + 1,
							0
						>::Result };
			};
			template <typename L1, typename L2> struct _CountCompatibles<0, L1, L2>
			{
				enum {Result = 0};
			};

			enum { LengthA = sizeof...(Types), LengthB = sizeof...(Ts) };
			enum { MinRange = DKCondEnum < (LengthA < LengthB), LengthA, LengthB>::Result };

			using _List1 = DKTypeList<Types...>;
			using _List2 = DKTypeList<Ts...>;

			enum {NumCompatibles = _CountCompatibles<MinRange, _List1, _List2>::Result};
			enum {Result = NumCompatibles == LengthA && NumCompatibles == LengthB};
		};
		// 특정 타입 모두 제거
		template <typename... Ts> struct _Remove
		{
			using Result = DKTypeList<Types...>;
		};
		template <typename T, typename... Ts> struct _Remove<T, Ts...>
		{
			// T1 과 같은것만 빼고 리스트 재구성함.
			template <typename T1, typename... Ts2> struct _EraseOne
			{
				using Result = DKTypeList<>;
			};
			template <typename T1, typename... Ts2> struct _EraseOne<T1,T1,Ts2...>
			{
				using Result = typename _EraseOne<T1, Ts2...>::Result;
			};
			template <typename T1, typename T2, typename... Ts2> struct _EraseOne<T1,T2,Ts2...>
			{
				using _Rest = typename _EraseOne<T1, Ts2...>::Result;
				using Result = typename DKTypeList<T2>::template Append< _Rest >;
			};

			using R = typedef typename _EraseOne<T, Types...>::Result;
			using Result = typename R::template _Remove<Ts...>::Result;
		};
		// 특정위치의 타입만 제거
		template <unsigned Index> struct _RemoveIndex
		{
			template <unsigned N, typename... Ts> struct _RemoveAt
			{
				using Result = DKTypeList<>;
			};
			template <unsigned N, typename T, typename... Ts> struct _RemoveAt<N, T, Ts...>
			{
				using _Rest = typename _RemoveAt<N+1, Ts...>::Result;
				using Result = typename DKTypeList<T>::template Append<_Rest>;
			};
			template <typename T, typename... Ts> struct _RemoveAt<Index, T, Ts...>
			{
				using Result = typename _RemoveAt<Index+1, Ts...>::Result;
			};

			static_assert(Index < sizeof...(Types), "Index must be less than count");
			using Result = typename _RemoveAt<0, Types...>::Result;
		};
		// 서브 리스트 생성
		template <unsigned Begin, unsigned End> struct _SubListT
		{
			static_assert(Begin < sizeof...(Types), "Index must be less than count");
			static_assert(End < sizeof...(Types), "Out of range");
			static_assert(Begin <= End, "Invalid range");

			template <unsigned, typename...> struct _SubList;
			template <unsigned Index, typename T, typename... Ts> struct _SubList<Index, T, Ts...>
			{
				using _Rest = typename _SubList<Index+1, Ts...>::Result;
				using Result = DKCondType< Index < Begin, _Rest, typename DKTypeList<T>::template Append< _Rest > >;
			};
			template <typename T, typename... Ts> struct _SubList<End, T, Ts...>
			{
				using Result = DKTypeList<T>;
			};

			using Result = typename _SubList<0, Types...>::Result;
		};

		// 특정 위치에 타입들 추가
		template <unsigned Index, typename... Ts> struct _InsertTypesAt
		{
			static_assert( Index <= sizeof...(Types), "Index must be in range" );

			template <unsigned, typename...> struct _InsertT;
			template <unsigned N, typename T, typename... Ts2> struct _InsertT<N, T, Ts2...>
			{
				using Result = typename DKTypeList<T>::template Append< typename _InsertT<N+1, Ts2...>::Result >;
			};
			template <typename T, typename... Ts2> struct _InsertT<Index, T, Ts2...>
			{
				using Result = DKTypeList<Ts..., T, Ts2...>;
			};
			template <typename... Ts2> struct _InsertT<Index, Ts2...>
			{
				using Result = DKTypeList<Ts..., Ts2...>;
			};

			using Result = typename _InsertT<0, Types...>::Result;
		};

		// 역순으로 리스트 재구성
		template <typename... Ts> struct _Reverse
		{
			using Result = DKTypeList<>;
		};
		template <typename T, typename... Ts> struct _Reverse<T, Ts...>
		{
			using R = typename _Reverse<Ts...>::Result;
			using Result = typename R::template Append<T>;
		};

	public:
		enum {Length = sizeof...(Types)};

		template < template <typename...> class T > using TypesInto = T<Types...>;

		// 역순 리스트
		using Reverse = typename _Reverse<Types...>::Result;

		// Index 의 타입을 정의함
		template <unsigned Index> using TypeAt = typename _TypeAt<Index>::Result;

		// 해당타입의 위치(인덱스)를 구함
		template <typename T> using IndexOf = DKNumber<_IndexOf<T>::Result>;

		// 타입T 가 리스트에 몇개 있는지 구함
		template <typename T> using Count = DKNumber<_CountType<T>::Result>;

		// 리스트 뒤에 타입들 추가
		// 주의: DKTypeList<..> 를 다른 타입과 함께 넣으면 하나의 타입으로만 간주한다.
		template <typename... Ts> using Append = typename _Append<Ts...>::Result;

		// 특정 위치에 타입 추가 (DKTypeList 도 하나의 타입으로 간주한다. 풀어서 넣으려면 Append 사용할것)
		template <unsigned Index, typename... Ts> using InsertAt = typename _InsertTypesAt<Index, Ts...>::Result;
		// 리스트가 같은지 확인.
		template <typename... Ts> using IsSame = DKCondType<_IsSame<Ts...>::Result, DKTrue, DKFalse>;

		// 리스트 타입을 변형 가능한지 확인.
		template <typename... Ts> using IsConvertible = DKCondType<_IsConvertible<Ts...>::Result, DKTrue, DKFalse>;

		// 특정 타입 모두 제거
		template <typename... Ts> using Remove = typename _Remove<Ts...>::Result;
		
		// 특정 위치 타입 한개 제거
		template <unsigned Index> using RemoveAt = typename _RemoveIndex<Index>::Result;
		
		// 서브 리스트 생성
		template <unsigned Begin, unsigned End> using SubList = typename _SubListT<Begin, End>::Result;
	};
}
