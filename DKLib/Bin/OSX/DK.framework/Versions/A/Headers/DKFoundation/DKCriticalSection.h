//
//  File: DKCriticalSection.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKCriticalSection<T>
//
// 동기화 객체 (DKLock, DKMutex, DKSpinLock, DKSharedLock 등) 를 사용하여
// 객체의 유효범위(scope) 내에서 임계영역을 보장하는 객체
//
//  if ( .. )
//  {
//      DKCriticalSection<DKSpinLock> section(this->myLock);
//
//      .. section 객체가 유지되는 동안은 임계영역
//   }
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	template <typename T> class DKCriticalSection
	{
	public:
		DKCriticalSection(const T& lockObject)
			: lock(lockObject)
		{
			lock.Lock();
		}
		~DKCriticalSection(void)
		{
			lock.Unlock();
		}
	private:
		DKCriticalSection(const DKCriticalSection&);
		DKCriticalSection& operator = (const DKCriticalSection&);
		const T& lock;
	};
}
