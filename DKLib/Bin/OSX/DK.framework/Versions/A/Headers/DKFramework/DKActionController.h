//
//  File: DKActionController.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKModel.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKActionController
//
// DKModel (DKCollisionObject) 에 물리적 특성을 부여한다.
// DKDynamicsScene 에서만 사용할 수 있다.
//
// Note: 개별 객체의 애니메이션은 DKAnimationController 를 사용할것
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKActionController : public DKModel
	{
	public:
		DKActionController(void) : DKModel(TypeAction) {}
		virtual ~DKActionController(void) {}
		virtual void Update(double timeDelta, DKFoundation::DKTimeTick tick) = 0;
	};
}
