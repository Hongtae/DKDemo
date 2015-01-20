//
//  File: DKCondition.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKCondition
//
// 쓰레드 동기화에 사용되며, 한 쓰레드에서 시그널을 넣어서 다른 쓰레드를 깨울 수 있다.
// 사용할때 항상 Lock() 을 호출하여 객체를 잠근후에 사용이 끝나면 Unlock() 을 호출하여 잠금을 해제해야 한다.
// (참고: Wait(), WaitTimeout() 을 호출하면 락이 풀리며 함수가 리턴될때 다시 락이 잠긴다.)
//
// 쓰레드1:
// cond.Lock();
// value = false;	// 다른 쓰레드와 공유되는 값을 수정함
// cond.Signal();	// 다른 쓰레드 언락
// cond.Unlock();	// cond 언락. 이후 다른 쓰레드가 깨어난다.
//
// 쓰레드2:
// cond.Lock();		// 락을 건다
// while ( value )
//    cond.Wait();	// cond 가 Signal 상태가 될때까지 기다린다. (다른 스레드에서 c 를 설정함)
// 기타 작업..
// cond.Unlock();	// 락을 푼다
//
// 타임아웃 사용방법
// cond.Lock();		// 락을 건다
// if (cond.WaitTimeout(3.0))	// 3초간 기다림
// {
//		// 시그널 상태.. 
// }
// else
// {
//		// 타임아웃 (시그널 상태 아님)
// }
// cond.Unlock();	// 락을 푼다.
//
// 주의: Lock 을 건 쓰레드에서만 Unlock 해야한다!
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKCondition
	{
	public:
		DKCondition(void);
		~DKCondition(void);
		
		void Wait(void) const;					// Signal 상태가 될때까지 무기한 기다린다.
		bool WaitTimeout(double t) const;		// Signal 상태가 될때까지 t 초동안 기다린다. (signal 상태면 true 리턴)
		void Signal(void) const;
		void Broadcast(void) const;
		
		void Lock(void) const;
		bool TryLock(void) const;
		void Unlock(void) const;

	private:
		DKCondition(const DKCondition&);					// 복사를 막기 위해 private 로 선언
		DKCondition& operator = (const DKCondition&);		// 복사를 막기 위해 private 로 선언
		void* impl;
	};
}
