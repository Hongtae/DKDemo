//
//  File: DKSharedLock.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"

////////////////////////////////////////////////////////////////////////////////
// DKSharedLock
//
// 두종류의 Lock 을 지원한다.
// LockShared (Read-lock, shared), 여러 쓰레드가 공유할 수 있음
// Lock (Write-lock, exclusive), 한 쓰레드만 락을 가짐
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKSharedLock
	{
	public:
		DKSharedLock(void);
		~DKSharedLock(void);
		void LockShared(void) const;
		bool TryLockShared(void) const;
		void UnlockShared(void) const;
		void Lock(void) const;		// exclusive
		bool TryLock(void) const;	// exclusive
		void Unlock(void) const;	// exclusive

	private:
		DKSharedLock(const DKSharedLock&);					// 복사를 막기 위해 private 로 선언
		DKSharedLock& operator = (const DKSharedLock&);		// 복사를 막기 위해 private 로 선언
		void* impl;
	};

	// shared lock 의 critical-section 객체
	// exclusive lock 은 그냥 DKCriticalSection 사용
	class DKSharedLockReadOnlySection
	{
	public:
		DKSharedLockReadOnlySection(const DKSharedLock& sl) : lock(sl) { lock.LockShared(); }
		~DKSharedLockReadOnlySection(void) { lock.UnlockShared(); }

	private:
		DKSharedLockReadOnlySection(const DKSharedLockReadOnlySection&);
		DKSharedLockReadOnlySection& operator = (const DKSharedLockReadOnlySection&);
		const DKSharedLock& lock;
	};
}
