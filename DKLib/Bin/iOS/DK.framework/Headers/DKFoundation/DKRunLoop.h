//
//  File: DKRunLoop.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKObject.h"
#include "DKThread.h"
#include "DKOperation.h"
#include "DKDateTime.h"
#include "DKQueue.h"
#include "DKSpinLock.h"
#include "DKArray.h"
#include "DKOrderedArray.h"
#include "DKTimer.h"
#include "DKCondition.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKRunLoop
//
// 현재 쓰레드 또는 다른 쓰레드에서 런루프를 실행한다.
//
// Run 을 호출하여 새 쓰레드에서 런루프가 돌고 있는 경우에는 새 쓰레드에도 객체의 소유권이 생긴다.
// 따라서 객체를 완전히 소거하려면 명시적으로 Terminate()를 꼭 호출해야 한다.
//
// 메인 쓰레드가 종료될때는 꼭 Terminate(true) 를 호출하여 종료를 기다려야 한다.
//
// 개별적으로 처리하는 함수 ProcessOne, ProcessInTime 을 사용하려면, 반드시 Run 을 통해 생성된 
// 쓰레드 내부에서 사용해야 한다. (IsWorkingThread 가 true 를 리턴하는 쓰레드여야 함)
//
//
// PostOperation 에는 두가지 버전이 있다.
//  tick 기준: 시스템 틱 기반으로 현재로부터 몇 초 후 실행 여부 판단. 시스템 시간과 상관 없음
//  time 기준: 날짜 시간 기준으로 실행 여부 판단. 시스템 시간이 바뀌면 거기에 따른다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKRunLoop
	{
	public:
		struct OperationResult
		{
			virtual ~OperationResult(void) {}
			virtual bool Result(void) const = 0;	// wait until done
			virtual bool Revoke(void) const = 0;
			virtual bool IsDone(void) const = 0;
			virtual bool IsRevoked(void) const = 0;
			virtual bool IsPending(void) const = 0;
		};

		DKRunLoop(void);
		virtual ~DKRunLoop(void);

		bool IsRunning(void) const;
		bool IsWrokingThread(void) const;
		virtual bool Run(void);
		virtual void Terminate(bool wait);

		bool ProcessOne(bool processIdle);
		size_t ProcessInTime(size_t maxCmd, double timeout);
		void WaitNextLoop(void);
		bool WaitNextLoopTimeout(double t);

		// PostOperation: operation 을 주고 바로 리턴한다.
		DKObject<OperationResult> PostOperation(const DKOperation* operation, double delay = 0);			// tick base
		DKObject<OperationResult> PostOperation(const DKOperation* operation, const DKDateTime& runAfter);	// time base

		// ProcessOperation: operation 을 수생하고 나서 리턴한다.
		bool ProcessOperation(const DKOperation* operation);

		static DKRunLoop* CurrentRunLoop(void);							// 현재 쓰레드에 바인딩 된 RunLoop 리턴함.
		static DKRunLoop* RunLoopForThreadID(DKThread::ThreadId id);	// 해당 쓰레드의 런루프
		static bool IsRunning(DKRunLoop* runloop);

	protected:
		virtual void OnInitialize(void);
		virtual void OnTerminate(void);
		virtual void OnIdle(void);
		virtual void PerformOperation(const DKOperation* operation);

	private:
		size_t RevokeAllOperations(void);
		void RunLoopProc(void);
		bool GetNextLoopIntervalNL(double*) const;

		struct InternalCommand
		{
			DKObject<DKOperation>		operation;
			DKObject<OperationResult>	result;

			virtual ~InternalCommand(void) {}
		};
		struct InternalCommandTick : public InternalCommand { DKTimer::Tick fire; };
		struct InternalCommandTime : public InternalCommand { DKDateTime fire; };
		void InternalPostCommand(const InternalCommandTick& cmd);
		void InternalPostCommand(const InternalCommandTime& cmd);

		DKCondition								commandQueueCond;
		DKOrderedArray<InternalCommandTick>		commandQueueTick;
		DKOrderedArray<InternalCommandTime>		commandQueueTime;

		//DKSpinLock				internalQueueLock;
		//DKArray<InternalCommand>	internalQueue;
		//size_t					internalQueueIndex;

		DKObject<DKThread>	thread;
		DKThread::ThreadId	threadId;
		DKSpinLock			threadLock;
		bool				terminate;

		static bool InternalCommandCompareOrder(const InternalCommandTick&, const InternalCommandTick&);
		static bool InternalCommandCompareOrder(const InternalCommandTime&, const InternalCommandTime&);
	};
	typedef DKRunLoop::OperationResult DKRunLoopOperationResult;
}
