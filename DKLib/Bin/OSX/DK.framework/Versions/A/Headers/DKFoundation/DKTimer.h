//
//  File: DKTimer.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKTimer
// 고해상도 타이머, 플랫폼에 따라 고해상도 지원 안할 수 있음
//
// Frequency : 1초당 틱 카운트
//  - time = tick / frequency
//  - tick = time * frequency
//
// 주의:
//  시간 환산할 때 tick(64비트)가 double(가수부 52비트)의 정밀도 보다 크기 때문에 오차가 생길 수 있다.
//  정밀한 계산이 필요할때는 그냥 DKTimeTick64 단위를 사용할 것.
//  (DKTimer::Reset, DKTimer::Elapsed 는 기준시간 부터의 경과시간을 측정하기 때문에 오차가 작음)
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	typedef unsigned long		DKTimeTick;		// 일반 카운터
	typedef unsigned int		DKTimeTick32;	// 32비트 카운터
	typedef unsigned long long	DKTimeTick64;	// 64비트 카운터

	class DKLIB_API DKTimer
	{
	public:
		typedef DKTimeTick64 Tick;
		DKTimer(void);
		~DKTimer(void);

		double Reset(void);				// 타이머 리셋, 지난 리셋부터 경과시간 리턴
		double Elapsed(void) const;		// 타이머 경과시간 리턴

		static Tick SystemTick(void);				// 시스템 틱
		static Tick SystemTickFrequency(void);		// 초당 틱
	private:
		Tick timeStamp;
	};
}
