//
//  File: DKTuple.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include <type_traits>
#include "../DKInclude.h"
#include "DKTypeList.h"
#include "DKTypeTraits.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKTuple
//
// 튜플 객체
//
// 예제:
// 1. 객체 생성후 값 입력
//   DKTuple<int, float, const char*> myTuple;
//   myTuple.SetValue<0>(2, 3.14f, "tuple");
//
// 1.a 하나씩 별개로 사용도 가능하다.
//   myTuple.SetValue<0>(2);
//   myTuple.SetValue<1>(3.14f);
//   myTuple.SetValue<2>("tuple");
//
// 2. DKTupleMake 사용
//   auto myTuple = DKTupleMake(2, 3.14f, "tuple");
//
// 3. 생성자 사용
//   DKTuple<int, float, const char*> myTuple(DKTupleValueSet(), 2, 3.14f, "tuple");
//
// 값을 꺼낼때
//   int n = myTuple.Value<0>();
//   float f = myTuple.Value<1>();
//   const char* str = myTuple.Value<2>();
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	////////////////////////////////////////////////////////////////////////////////
	// 실제 값들이 저장되는 유닛 (레퍼런스는 포인터 형태로 저장된다)
	template <typename T> struct DKTupleUnit
	{
		using ValueType =	typename DKTypeTraits<T>::UnqualifiedType;
		using RefType = T&;
		using CRefType = const T&;

		ValueType value;

		DKTupleUnit(void) {}
		DKTupleUnit(const T& v) : value(v) {}
		DKTupleUnit(T&& v) : value(static_cast<T&&>(v)) {}
		DKTupleUnit(const DKTupleUnit& t) : value(t.value) {}
		DKTupleUnit(DKTupleUnit&& t) : value(static_cast<ValueType&&>(t.value)) {}

		void SetValue(const T& v)	{this->value = v;}
		void SetValue(T&& v)		{this->value = static_cast<T&&>(v);}
		RefType Value(void)			{return this->value;}
		CRefType Value(void) const	{return this->value;}

		DKTupleUnit& operator = (const T& v)
		{
			value = v;
			return *this;
		}
		DKTupleUnit& operator = (const DKTupleUnit& t)
		{
			value = t.value;
			return *this;
		}
		DKTupleUnit& operator = (T&& v)
		{
			this->value = static_cast<T&&>(v);
			return *this;
		}
		DKTupleUnit& operator = (DKTupleUnit&& t)
		{
			this->value = static_cast<ValueType&&>(t.value);
			return *this;
		}
	};
	template <typename T> struct DKTupleUnit<T&>
	{
		using ValueType = typename DKTypeTraits<T>::UnqualifiedType*;
		using RefType = T&;
		using CRefType = const T&;

		ValueType value;

		DKTupleUnit(void) : value(NULL) {}
		DKTupleUnit(const DKTupleUnit& t) : value(t.value) {}
		DKTupleUnit(DKTupleUnit&& t) : value(t.value) {t.value = NULL;}
		DKTupleUnit(T& v) : value(const_cast<ValueType>(&v)) {}

		void SetValue(T& v)			{this->value = const_cast<ValueType>(&v);}
		RefType Value(void)			{return *this->value;}
		CRefType Value(void) const	{return *this->value;}

		DKTupleUnit& operator = (T& v)
		{
			this->value = const_cast<ValueType>(&v);
			return *this;
		}
		DKTupleUnit& operator = (const DKTupleUnit& t)
		{
			value = t.value;
			return *this;
		}
		DKTupleUnit& operator = (DKTupleUnit&& t)
		{
			this->value = t.value;
			t.value = NULL;
			return *this;
		}
	};
	template <typename T> struct DKTupleUnit<T&&>
	{
		using ValueType = typename DKTypeTraits<T>::UnqualifiedType;
		using RefType = T&&;
		using CRefType = const T&;

		ValueType value;

		DKTupleUnit(void) {}
		DKTupleUnit(const DKTupleUnit& t) : value(t.value) {}
		DKTupleUnit(DKTupleUnit&& t) : value(static_cast<ValueType&&>(t.value)) {}
		DKTupleUnit(T&& v) : value(static_cast<T&&>(v)) {}

		void SetValue(T&& v)		{this->value = static_cast<T&&>(v);}
		RefType Value(void)			{return static_cast<RefType>(this->value);}
		CRefType Value(void) const	{return static_cast<CRefType>(this->value);}

		DKTupleUnit& operator = (T&& v)
		{
			this->value = static_cast<T&&>(v);
			return *this;
		}
		DKTupleUnit& operator = (const DKTupleUnit& t)
		{
			value = t.value;
			return *this;
		}
		DKTupleUnit& operator = (DKTupleUnit&& t)
		{
			this->value = static_cast<ValueType&&>(t.value);
			return *this;
		}
	};

	struct DKTupleValueSet {};
	template <typename... Types> class DKTuple
	{
		// DKTupleUnit 을 포함하는 데이터 유닛 객체 (각각의 타입은 인덱스로 구별함)
		// 객체 하나가 모든 타입을 포함해야 한다 (상속)
		template <size_t Level, typename... Ts> struct DataUnitHierarchy
		: public DataUnitHierarchy<Level-1, Ts...>
		{
			using Super = DataUnitHierarchy<Level-1, Ts...>;
			using TypeList = DKTypeList<Ts...>;
			enum {Index = TypeList::Length - Level};
			static_assert(Index >= 0, "Index must be greater or equal to zero");
			using UnitType = typename TypeList::template TypeAt<Index>;
			using TupleUnit = DKTupleUnit<UnitType>;

			DataUnitHierarchy(void) {}
			DataUnitHierarchy(const DataUnitHierarchy& d) : Super(d), unit(d.unit) {}
			DataUnitHierarchy(DataUnitHierarchy&& d) : Super(static_cast<Super&&>(d)), unit(static_cast<TupleUnit&&>(d.unit)) {}
			// 값을 직접 생성자로 넘겨주려면 첫번째 인자에 DKTupleValueSet 를 넘겨줘야 한다.
			// (매개변수가 임의의 형식이므로 다른 생성자와 다르게 오버로딩 해야함)
			template <typename V, typename... Vs>
			DataUnitHierarchy(const DKTupleValueSet& tv, V&& v1, Vs&&... vs) : Super(tv, std::forward<Vs>(vs)...), unit(std::forward<V>(v1)) {}
			DataUnitHierarchy(const DKTupleValueSet&) {}

			DataUnitHierarchy& operator = (const DataUnitHierarchy& d)
			{
				Super::operator=(d);
				unit = d.unit;
				return *this;
			}
			DataUnitHierarchy& operator = (DataUnitHierarchy&& d)
			{
				Super::operator=(static_cast<Super&&>(d));
				unit = static_cast<TupleUnit&&>(d.unit);
				return *this;
			}
			TupleUnit unit;
		};
		template <typename... Ts> struct DataUnitHierarchy<0, Ts...> // 최상위 객체
		{
			DataUnitHierarchy(void)						{}
			DataUnitHierarchy(const DKTupleValueSet&)	{}
		};
		// Index 로 접근 가능한 타입
		template <size_t Index> using DataUnitAtIndex = DataUnitHierarchy<sizeof...(Types) - Index, Types...>;
		using DataUnitType = DataUnitHierarchy<sizeof...(Types), Types...>;

		DataUnitType dataUnits;

	public:
		enum {Length = sizeof...(Types)};
		using TypeList = DKTypeList<Types...>;

		template <size_t Index> using TypeAt = typename TypeList::template TypeAt<Index>;
		template <typename T> using IndexOf = typename TypeList::template IndexOf<T>;

		template <typename T> static constexpr auto HasType(void)->bool
		{
			return TypeList::template Count<T>::Value > 0;
		};
		
		template <size_t Index> auto Unit(void) -> DKTupleUnit<TypeAt<Index>>&
		{
			static_assert(Index < sizeof...(Types), "Index must be lesser than type size");
			return static_cast<DataUnitAtIndex<Index>&>(dataUnits).unit;
		};
		template <size_t Index> auto Unit(void) const -> const DKTupleUnit<TypeAt<Index>>&
		{
			static_assert(Index < sizeof...(Types), "Index must be lesser than type size");
			return static_cast<const DataUnitAtIndex<Index>&>(dataUnits).unit;
		};
		template <size_t Index> auto Value(void) -> typename DKTupleUnit<TypeAt<Index>>::RefType
		{
			static_assert(Index < sizeof...(Types), "Index must be lesser than type size");
			return Unit<Index>().Value();
		};
		template <size_t Index> auto Value(void) const -> typename DKTupleUnit<TypeAt<Index>>::CRefType
		{
			static_assert(Index < sizeof...(Types), "Index must be lesser than type size");
			return Unit<Index>().Value();
		};

		template <size_t Index, typename... Ts> void SetValue(Ts&&...) {}
		template <size_t Index, typename T, typename... Ts> void SetValue(T&& v, Ts&&... vs)
		{
			static_assert(Index < sizeof...(Types)," Index must be lesser than type size");
			Unit<Index>().SetValue(std::forward<T>(v));
			SetValue<Index+1, Ts...>(std::forward<Ts>(vs)...);
		}

		DKTuple(void) {}
		DKTuple(const DKTuple& t) : dataUnits(t.dataUnits) {}
		DKTuple(DKTuple&& t) : dataUnits(static_cast<DataUnitType&&>(t.dataUnits)) {}
		// 임의의 값을 세팅하려면 DKTupleValueSet 매개변수를 사용하여야 한다.
		// 매개변수가 임의의 형식이므로 DKTupleValueSet 을 이용하여 오버로드 해야한다.
		template <typename... Ts> DKTuple(const DKTupleValueSet& tv, Ts&&... vs) : dataUnits(tv, std::forward<Ts>(vs)...)
		{
			static_assert(sizeof...(Ts) <= Length, "Invalid arguments");
		}

		DKTuple& operator = (const DKTuple& t)
		{
			dataUnits = t.dataUnits;
			return *this;
		}
		DKTuple& operator = (DKTuple&& t)
		{
			if (this != &t)
				dataUnits = static_cast<DataUnitType&&>(t.dataUnits);
			return *this;
		}
	};

	template <typename... Types> auto DKTupleMake(Types&&... vs)-> DKTuple<Types...>
	{
		return DKTuple<Types...>(DKTupleValueSet(), std::forward<Types>(vs)...);
	}
}
