//
//  File: DKMutex.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKMutex
//
// 일반적인 뮤텍스 객체 (Recursive 하지 않음)
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKMutex
	{
	public:
		DKMutex(void);
		~DKMutex(void);
		void Lock(void) const;
		bool TryLock(void) const;
		void Unlock(void) const;

	private:
		DKMutex(const DKMutex&);					// 복사를 막기 위해 private 로 선언
		DKMutex& operator = (const DKMutex&);		// 복사를 막기 위해 private 로 선언
		void* impl;
	};
}
