//
//  File: DKThread.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKObject.h"
#include "DKOperation.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKThread
//
// 새 쓰레드를 생성하고 DKOperation 을 실행한다.
// DKThread::Create 을 통해서 객체를 얻을수 있으며, 얻은 객체의 소유권을 포기해도 객체는 남아있다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKThread
	{
	public:
		typedef unsigned long ThreadId;

		void WaitTerminate(void) const;
		ThreadId Id(void) const;
		bool IsAlive(void) const;

		static DKObject<DKThread> FindThread(ThreadId id);
		static DKObject<DKThread> CurrentThread(void);
		static ThreadId CurrentThreadId(void);		// 현재 쓰레드 ID 가져오기
		static void Yield(void);					// CPU우선권 양보
		static void Sleep(double d);				// 현재 쓰레드 재우기

		static DKObject<DKThread> Create(const DKOperation* op);
		static const ThreadId invalidId;

	private:
		DKThread(void);
		~DKThread(void);

		DKThread(const DKThread&);
		DKThread& operator = (const DKThread&);

		mutable ThreadId threadId;
		friend class DKObject<DKThread>;
	};
}
