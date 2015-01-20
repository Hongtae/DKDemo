//
//  File: DKSoftBody.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2012-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "../DKFoundation.h"
#include "DKCollisionObject.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKSoftBody
// 연체 물리 인터페이스
//
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKSoftBody : public DKCollisionObject
	{
	public:
		DKSoftBody(void);
		~DKSoftBody(void);

		DKCollisionShape* CollisionShape(void)				{return NULL;}
		const DKCollisionShape* CollisionShape(void) const	{return NULL;}

		DKFoundation::DKObject<DKSerializer> Serializer(void) override;

	protected:
		DKFoundation::DKObject<DKModel> Clone(UUIDObjectMap&) const override;
	};
}
