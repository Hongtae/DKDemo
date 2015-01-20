//
//  File: DKDummyLock.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"

namespace DKFoundation
{
	class DKDummyLock
	{
	public:
		DKDummyLock(void) {}
		~DKDummyLock(void) {}
		void Lock(void) const {}
		bool TryLock(void) const {return true;}
		void Unlock(void) const {}
	private:
		DKDummyLock(const DKDummyLock&);					// 복사를 막기 위해 private 로 선언
		DKDummyLock& operator = (const DKDummyLock&);		// 복사를 막기 위해 private 로 선언
	};
}
