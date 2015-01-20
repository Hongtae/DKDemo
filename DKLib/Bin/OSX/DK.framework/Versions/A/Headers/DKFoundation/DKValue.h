//
//  File: DKValue.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKTypeTraits.h"
#include "DKObject.h"
#include "DKInvocation.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKValue
//
// 변수나 상수를 바인딩 하여 DKInvocation 객체로 만든다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	template <typename T> class DKInvocationValue : public DKInvocation<T>
	{
	public:
		DKInvocationValue(T v) : value(v) 
		{
		}
	protected:
		T Invoke(void) const
		{
			return value;
		}
	private:
		T value;
	};
	template <typename T> class DKInvocationValue<T*> : public DKInvocation<T*>
	{
	public:
		DKInvocationValue(T* v) : value(v) 
		{
		}
	protected:
		T* Invoke(void) const
		{
			return const_cast<T*>((const T*)value);
		}
	private:
		DKObject<T> value;
	};

	template <typename T> DKObject<DKInvocation<T>> DKValue(T value)
	{
		DKObject<DKInvocationValue<T>> invocation = DKOBJECT_NEW DKInvocationValue<T>(value);
		return invocation.template SafeCast<DKInvocation<T>>();
	}
}
