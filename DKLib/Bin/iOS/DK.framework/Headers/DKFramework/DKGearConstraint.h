//
//  File: DKGeneric6DofConstraint.h
//  Encoding: UTF-8 ☃
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2013-2014 ICONDB.COM. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKConstraint.h"

////////////////////////////////////////////////////////////////////////////////
//
// DKGearConstraint
//
// 두개의 rigid-body 가 서로 맞물려 회전할 수 있도록 한다.
////////////////////////////////////////////////////////////////////////////////

namespace DKFramework
{
	class DKLIB_API DKGearConstraint : public DKConstraint
	{
	public:
		DKGearConstraint(DKRigidBody* bodyA, DKRigidBody* bodyB,
						 const DKVector3& axisInA, const DKVector3& axisInB,
						 float ratio = 1.0f);
		DKGearConstraint(void);
		~DKGearConstraint(void);

		void SetAxisInA(const DKVector3& axis);
		void SetAxisInB(const DKVector3& axis);
		DKVector3 AxisInA(void) const;
		DKVector3 AxisInB(void) const;

		float Ratio(void) const;
		void SetRatio(float r);
		
		DKFoundation::DKObject<DKSerializer> Serializer(void) override;

	protected:
		void ResetContext(void) override;
		void ResetContextImpl(void) override;

		DKFoundation::DKObject<DKModel> Clone(UUIDObjectMap&) const override;
		DKGearConstraint* Copy(UUIDObjectMap&, const DKGearConstraint*);
	};
}
