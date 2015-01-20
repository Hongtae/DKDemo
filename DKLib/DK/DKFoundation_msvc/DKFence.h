//
//  File: DKFence.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"

////////////////////////////////////////////////////////////////////////////////
// DKFence
//
// 선언 필요없이 바로 함수처럼 사용하는 동기화 객체, DKCriticalSection 과 같이 사용할수 없다
// 해당 객체가 있는동안은 락이 유효하기 때문에, 동기화 객체 선언 없이 간단히 사용할때 유용함
//
//	if ( true )
//	{
//		DKFence fence(this);	// this 를 이용한 락은 여기서 블록된다.
//		.. // 이후의 연산은 크리티컬 섹션이다.
//		..
//	}
//
// Note: 오버헤드는 DKSpinLock 보다 조금 더 크다.
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	class DKLIB_API DKFence
	{
	public:
		DKFence(const void* key, bool exclusive = false);
		~DKFence(void);
		
	private:
		DKFence(const DKFence&);					// 복사를 막기 위해 private 로 선언
		DKFence& operator = (const DKFence&);		// 복사를 막기 위해 private 로 선언
		const void* key;
	};
}
