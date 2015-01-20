//
//  File: DKRunLoopTimer.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKObject.h"
#include "DKOperation.h"
#include "DKRunLoop.h"
#include "DKDateTime.h"
#include "DKTimer.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKRunLoopTimer
//
// DKRunLoop 에 타이머를 설치하여, 주기적으로 operation 을 반복 실행할수 있도록 한다.
// 시스템에 따라서 미세한 오차가 있을 수 있다.
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKRunLoopTimer
	{
	public:
		~DKRunLoopTimer(void);

		static DKObject<DKRunLoopTimer> Create(const DKOperation* operation, double interval, DKRunLoop* runLoop = NULL);

		size_t Count(void) const;		
		DKRunLoop* RunLoop(void) const;
		double Interval(void) const;
		bool IsRunning(void) const;

		void Invalidate(void);

	private:
		struct Invoker : public DKOperation
		{
			virtual void Invalidate(void) = 0;
			virtual size_t Count(void) const = 0;
			virtual DKRunLoop* RunLoop(void) const = 0;
			virtual bool IsRunning(void) const = 0;
			virtual double Interval(void) const = 0;
		};
		DKObject<Invoker> invoker;

		DKRunLoopTimer(void);
		friend class DKObject<DKRunLoopTimer>;
	};
}
