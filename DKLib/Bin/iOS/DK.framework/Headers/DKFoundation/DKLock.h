//
//  File: DKLock.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKLock (RecursiveLock)
// 
// Lock 을 건 쓰레드에서만 Unlock 할수 있다.
// Lock 을 걸고나서 또 Lock 을 걸수 있다. 이때 건 횟수만큼 Unlock 을 해줘야 풀린다.
// Lock 을 건 쓰레드와 Unlock 을 시도하는 쓰레드가 다를경우 예외처리 된다. DKError로 확인가능
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKLock
	{
	public:
		DKLock(void);
		~DKLock(void);
		void Lock(void) const;
		bool TryLock(void) const;
		void Unlock(void) const;

	private:
		DKLock(const DKLock&);					// 복사를 막기 위해 private 로 선언
		DKLock& operator = (const DKLock&);		// 복사를 막기 위해 private 로 선언
		void* impl;
	};
}
