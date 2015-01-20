//
//  File: DKSingleton.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKSpinLock.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKSingleton
//
// 런타임시 단 한개의 인스턴스만 존재하도록 한다.
//
// 상속받거나 typedef 로 정의하여 할 수 있다. 상속을 권장
////////////////////////////////////////////////////////////////////////////////

namespace DKFoundation
{
	template <class TYPE> class DKSingleton
	{
	public:
		static TYPE& Instance(void)
		{
			static TYPE* instancePtr = NULL;
			if (instancePtr == NULL)
			{
				static DKSpinLock lock;
				lock.Lock();
				if (instancePtr == NULL)
				{
					static TYPE obj;
					instancePtr = &obj;
				}
				lock.Unlock();
			}
			return *instancePtr;
		}
	private:
		DKSingleton(const DKSingleton&);
		DKSingleton& operator = (const DKSingleton&);
	protected:
		DKSingleton(void)					{}
		virtual ~DKSingleton(void)			{}
	};
}
