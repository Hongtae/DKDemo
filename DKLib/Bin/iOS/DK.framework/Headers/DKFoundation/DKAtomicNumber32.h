//
//  File: DKAtomicNumber32.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"

////////////////////////////////////////////////////////////////////////////////
// DKAtomicNumber32
// atomic 으로 수를 증가하거나 감소시킴
// 이 클래스는 사칙연산에 매우 민감하므로 assign 을 제외한 오퍼레이터는 제공하지 않음
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKAtomicNumber32
	{
	public:
		typedef int Value;
		DKAtomicNumber32(Value initialValue = 0);
		~DKAtomicNumber32(void);

		Value Increment(void);								// 1 증가, 이전 값 리턴.
		Value Decrement(void);								// 1 감소, 이전 값 리턴
		Value Add(Value addend);							// addend 만큼 증가. 이전 값 리턴.
		Value Exchange(Value value);						// value 를 새 값으로 설정후 이전값 리턴함.
		bool CompareAndSet(Value comparand, Value value);	// 비교 후 같으면 value로 설정

		DKAtomicNumber32& operator = (Value value);
		DKAtomicNumber32& operator += (Value value);
		operator Value (void) const;
	private:
		volatile Value atomic;
	};
}
