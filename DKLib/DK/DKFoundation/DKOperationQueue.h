//
//  File: DKOperationQueue.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKThread.h"
#include "DKOperation.h"
#include "DKQueue.h"
#include "DKCondition.h"
#include "DKSpinLock.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKOperationQueue
//
// 멀티쓰레딩으로 DKOperation 을 처리한다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKOperationQueue
	{
	public:
		struct OperationSync
		{
			enum State
			{
                StateUnknown = 0,
				StatePending,
				StateProcessed,
				StateCancelled,
			};

			virtual ~OperationSync(void) {}
			virtual bool Sync(void) = 0;
			virtual bool Cancel(void) = 0;
			virtual State OperationState(void) = 0;
		};

		class ThreadFilter
		{
			friend class DKOperationQueue;
		public:
			virtual ~ThreadFilter(void) {}
		protected:
			virtual void OnThreadInitialized(void) {}
			virtual void OnThreadTerminate(void) {}
			virtual void PerformOperation(DKOperation* op)
			{
				op->Perform();
			}
		};

		DKOperationQueue(ThreadFilter* filter = NULL);
		~DKOperationQueue(void);

		void SetMaxConcurrentOperations(size_t maxConcurrent);
		size_t MaxConcurrentOperations(void) const;

		void Post(DKOperation* operation);
		DKObject<OperationSync> ProcessAsync(DKOperation* operation);
		bool Process(DKOperation* operation);	// 실행 완료될때까지 기다림
		void CancelAllOperations(void);			// 모든 작업 취소
		void WaitForCompletion(void) const;		// 작업이 모두 완료되기를 기다림

		size_t QueueLength(void) const;
		size_t RunningOperations(void) const;
		size_t RunningThreads(void) const;

	private:
		struct Operation
		{
			DKObject<DKOperation> operation;
			DKObject<OperationSync> sync;
		};
		typedef DKQueue<Operation, DKDummyLock> OperationQueue;
		OperationQueue operationQueue;
		size_t maxConcurrentOperations;
		size_t threadCount;			// 가용 쓰레드
		size_t maxThreadCount;		// 최대 쓰레드
		size_t activeThreads;		// 현재 작업중
		DKCondition threadCond;
		DKObject<ThreadFilter> filter;

		void UpdateThreadPool(void);
		void OperationProc(void);

		DKOperationQueue(const DKOperationQueue&);
		DKOperationQueue& operator = (const DKOperationQueue&);
	};
}
