//
//  File: DKOperation.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKOperation
//
// 함수자 객체, 이 객체는 리턴값을 얻을 수 없다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKOperation
	{
	public:
		virtual ~DKOperation(void) {}
		virtual void Perform(void) const = 0;

	protected:
		DKOperation(void) {}

	private:
		DKOperation(DKOperation&&);
		DKOperation(const DKOperation&);
		DKOperation& operator = (DKOperation&&);
		DKOperation& operator = (const DKOperation&);
	};
}
