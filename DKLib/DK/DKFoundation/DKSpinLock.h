//
//  File: DKSpinLock.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKAtomicNumber32.h"

////////////////////////////////////////////////////////////////////////////////
// DKSpinLock
//
// 루프돌며 busy-waiting 을 한다. 아주 잠깐동안만 락을 걸때 사용한다.
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKSpinLock
	{
	public:
		DKSpinLock(void);
		~DKSpinLock(void);
		void Lock(void) const;
		bool TryLock(void) const;
		void Unlock(void) const;

	private:
		DKSpinLock(const DKSpinLock&);					// 복사를 막기 위해 private 로 선언
		DKSpinLock& operator = (const DKSpinLock&);		// 복사를 막기 위해 private 로 선언
		mutable DKAtomicNumber32 state;
	};
}
