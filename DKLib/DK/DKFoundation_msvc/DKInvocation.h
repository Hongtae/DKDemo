//
//  File: DKInvocation.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKTypes.h"
#include "DKOperation.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKInvocation<T>
//
//  이 객체는 DKOperation 으로 사용할 수 있으며, T 타입의 리턴값을 얻을 수 있다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	template <typename T> struct DKInvocation : public DKOperation
	{
		virtual ~DKInvocation(void) {}

		void Perform(void) const {Invoke();}		
		virtual T Invoke(void) const = 0;

		template <typename U> static bool CanAcceptResultTypeAs(void)
		{
			return DKTypeConversionTest<T, U>::Result;
		};
	};
}
